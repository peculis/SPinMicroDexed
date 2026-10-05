/*
   MicroDexed

   MicroDexed is a port of the Dexed sound engine
   (https://github.com/asb2m10/dexed) for the Teensy-3.5/3.6/4.x with audio shield.
   Dexed ist heavily based on https://github.com/google/music-synthesizer-for-android

   (c)2018-2021 H. Wirtz <wirtz@parasitstudio.de>

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software Foundation,
   Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301  USA

*/

#include <Arduino.h>
#include "config.h"
#include <Wire.h>
#include <SD.h>
#include "dexed.h"
#include "dexed_sd.h"

extern void init_MIDI_send_CC(void);

/******************************************************************************
   SD BANK/VOICE LOADING
 ******************************************************************************/

bool load_sd_voice(uint8_t b, uint8_t v, uint8_t instance_id)
{
  v = constrain(v, 0, MAX_VOICES - 1);
  b = constrain(b, 0, MAX_BANKS - 1);

  if (sd_card > 0)
  {
    File sysex;
    char filename[FILENAME_LEN];
    char bank_name[BANK_NAME_LEN];
    uint8_t data[128];

    if (!get_bank_name(b, bank_name, sizeof(bank_name)))
      return false;
    snprintf(filename, sizeof(filename), "/%u/%s.syx", unsigned(b), bank_name);

    sysex = SD.open(filename);
    if (!sysex)
    {
#ifdef DEBUG
      Serial.print(F("E : Cannot open "));
      Serial.print(filename);
      Serial.println(F(" on SD."));
#endif
      return (false);
    }

    if (get_sd_voice(sysex, v, data))
    {
#ifdef DEBUG
      char voice_name[VOICE_NAME_LEN];
      get_voice_name(b, v, voice_name, sizeof(voice_name));
      Serial.print(F("Loading voice from "));
      Serial.print(filename);
      Serial.print(F(" ["));
      Serial.print(voice_name);
      Serial.println(F("]"));
#endif
      bool ret = MicroDexed[instance_id]->decodeVoice(data, MicroDexed[instance_id]->data);
#ifdef DEBUG
      show_patch(instance_id);
#endif

      configuration.dexed[instance_id].transpose = MicroDexed[instance_id]->data[DEXED_VOICE_OFFSET + DEXED_TRANSPOSE];

      sysex.close();

      uint8_t data_copy[155];
      MicroDexed[instance_id]->getVoiceData(data_copy);
      send_sysex_voice(configuration.dexed[instance_id].midi_channel, data_copy);
      init_MIDI_send_CC();
      return (ret);
    }
#ifdef DEBUG
    else
      Serial.println(F("E : Cannot load voice data"));
#endif
    sysex.close();
  }

  return (false);
}

bool save_sd_voice(uint8_t b, uint8_t v, uint8_t instance_id)
{
  if (sd_card <= 0 || b >= MAX_BANKS || v >= MAX_VOICES || instance_id >= NUM_DEXED)
    return false;
  char bank_name[BANK_NAME_LEN];
  char filename[FILENAME_LEN];
  if (!get_bank_name(b, bank_name, sizeof(bank_name)))
    return false;
  snprintf(filename, sizeof(filename), "/%u/%s.syx", unsigned(b), bank_name);
  File original = SD.open(filename);
  uint8_t bank[4104];
  if (!original || original.read(bank, sizeof(bank)) != sizeof(bank))
  {
    original.close();
    return false;
  }
  original.close();
  if (!valid_dx7_bank(bank))
    return false;
  MicroDexed[instance_id]->encodeVoice(bank + 6 + 128 * v);
  bank[4102] = calc_checksum(bank + 6, 4096);
  return save_sd_bank(filename, bank);
}

bool get_sd_voice(File sysex, uint8_t voice_number, uint8_t* data)
{
  uint16_t n;
  int32_t bulk_checksum_calc = 0;
  int8_t bulk_checksum;

  if (sysex.size() != 4104) // check sysex size
  {
#ifdef DEBUG
    Serial.println(F("E : SysEx file size wrong."));
#endif
    return (false);
  }
  if (sysex.read() != 0xf0)  // check sysex start-byte
  {
#ifdef DEBUG
    Serial.println(F("E : SysEx start byte not found."));
#endif
    return (false);
  }
  if (sysex.read() != 0x43)  // check sysex vendor is Yamaha
  {
#ifdef DEBUG
    Serial.println(F("E : SysEx vendor not Yamaha."));
#endif
    return (false);
  }
  sysex.seek(4103);
  if (sysex.read() != 0xf7)  // check sysex end-byte
  {
#ifdef DEBUG
    Serial.println(F("E : SysEx end byte not found."));
#endif
    return (false);
  }
  sysex.seek(3);
  if (sysex.read() != 0x09)  // check for sysex type (0x09=32 voices)
  {
#ifdef DEBUG
    Serial.println(F("E : SysEx type not 32 voices."));
#endif
    return (false);
  }
  sysex.seek(4102); // Bulk checksum
  bulk_checksum = sysex.read();

  sysex.seek(6); // start of bulk data
  for (n = 0; n < 4096; n++)
  {
    uint8_t d = sysex.read();
    if (n >= voice_number * 128 && n < (voice_number + 1) * 128)
      data[n - (voice_number * 128)] = d;
    bulk_checksum_calc -= d;
  }
  bulk_checksum_calc &= 0x7f;

#ifdef DEBUG
  Serial.print(F("Bulk checksum : 0x"));
  Serial.print(bulk_checksum_calc, HEX);
  Serial.print(F(" [0x"));
  Serial.print(bulk_checksum, HEX);
  Serial.println(F("]"));
#endif

  if (bulk_checksum_calc != bulk_checksum)
  {
#ifdef DEBUG
    Serial.print(F("E : Bulk checksum mismatch : 0x"));
    Serial.print(bulk_checksum_calc, HEX);
    Serial.print(F(" != 0x"));
    Serial.println(bulk_checksum, HEX);
#endif
    return (false);
  }

  MicroDexed[0]->render_time_max = 0;

  return (true);
}

bool put_sd_voice(File sysex, uint8_t voice_number, uint8_t* data)
{
  // Retained helper: validate the original before any in-place update.
  uint8_t bank[4104];
  if (voice_number >= 32 || !sysex.seek(0) || sysex.size() != sizeof(bank) ||
      sysex.read(bank, sizeof(bank)) != sizeof(bank) || !valid_dx7_bank(bank))
    return false;
  memcpy(bank + 6 + 128 * voice_number, data, 128);
  bank[4102] = calc_checksum(bank + 6, 4096);
  if (!valid_dx7_bank(bank) || !sysex.seek(0) ||
      sysex.write(bank, sizeof(bank)) != sizeof(bank))
    return false;
  sysex.flush();
  return true;
}

bool save_sd_bank(const char* bank_filename, uint8_t* data)
{
  if (sd_card <= 0 || !valid_dx7_bank(data))
    return false;
  // Accept only the documented /0..99/<1..10 characters>.syx layout.
  unsigned bank_number;
  int offset = 0;
  if (sscanf(bank_filename, "/%u/%n", &bank_number, &offset) != 1 ||
      offset == 0 || bank_number >= MAX_BANKS)
    return false;
  const char* stem = bank_filename + offset;
  size_t length = strlen(stem);
  if (length < 5 || length > 14 || strcmp(stem + length - 4, ".syx") != 0 ||
      stem[0] == '.' || strchr(stem, '/') || strchr(stem, '\\'))
    return false;
  char directory_name[5];
  snprintf(directory_name, sizeof(directory_name), "/%u", bank_number);
  File directory = SD.open(directory_name);
  if (!directory || !directory.isDirectory())
    return false;
  while (true)
  {
    File entry = directory.openNextFile();
    if (!entry)
      break;
    const char* name = entry.name();
    const char* extension = strrchr(name, '.');
    bool conflict = !entry.isDirectory() && extension &&
        strcasecmp(extension, ".syx") == 0 && strcasecmp(name, stem) != 0;
    entry.close();
    if (conflict)
    {
      directory.close();
      return false;
    }
  }
  directory.close();
  return replace_sd_bank_file(bank_filename, data);
}

/******************************************************************************
   SD VOICECONFIG
 ******************************************************************************/
bool load_sd_voiceconfig(int8_t vc, uint8_t instance_id)
{
  if (vc < 0)
    return (false);

  vc = constrain(vc, 0, MAX_VOICECONFIG);

  if (sd_card > 0)
  {
    File sysex;
    char filename[FILENAME_LEN];

    sprintf(filename, "/%s/%s%d.syx", VOICE_CONFIG_PATH, VOICE_CONFIG_NAME, vc);

    // first check if file exists...
    if (SD.exists(filename))
    {
      // ... and if: load
#ifdef DEBUG
      Serial.print(F("Found voice configuration ["));
      Serial.print(filename);
      Serial.println(F("]... loading..."));
#endif
      sysex = SD.open(filename);
      if (sysex)
      {
        if (!get_sd_data(sysex, 0x42, (uint8_t*)&configuration.dexed[instance_id], sizeof(configuration.dexed[instance_id])))
        {
          sysex.close();
          return false;
        }
        set_voiceconfig_params(instance_id);
        sysex.close();

        return (true);
      }
#ifdef DEBUG
      else
      {
        Serial.print(F("E : Cannot open "));
        Serial.print(filename);
        Serial.println(F(" on SD."));
      }
#endif
    }
    else
    {
#ifdef DEBUG
      Serial.print(F("No "));
      Serial.print(filename);
      Serial.println(F(" available."));
#endif
    }
  }
  return (false);
}

bool save_sd_voiceconfig(uint8_t vc, uint8_t instance_id)
{
  vc = constrain(vc, 0, MAX_VOICECONFIG);

  if (sd_card > 0)
  {
    char filename[FILENAME_LEN];

    snprintf(filename, sizeof(filename), "/%s/%s%d.syx", VOICE_CONFIG_PATH, VOICE_CONFIG_NAME, vc);

#ifdef DEBUG
    Serial.print(F("Saving voice config "));
    Serial.print(vc);
    Serial.print(F("["));
    Serial.print(instance_id);
    Serial.print(F("]"));
    Serial.print(F(" to "));
    Serial.println(filename);
#endif

    return replace_sd_configuration(filename, 0x42, (uint8_t*)&configuration.dexed[instance_id], sizeof(configuration.dexed[instance_id]));
  }

  return (false);
}

/******************************************************************************
   SD FX
 ******************************************************************************/
bool load_sd_fx(int8_t fx)
{
  if (fx < 0)
    return (false);

  fx = constrain(fx, 0, MAX_FX);

  if (sd_card > 0)
  {
    File sysex;
    char filename[FILENAME_LEN];

    sprintf(filename, "/%s/%s%d.syx", FX_CONFIG_PATH, FX_CONFIG_NAME, fx);

    // first check if file exists...
    if (SD.exists(filename))
    {
      // ... and if: load
#ifdef DEBUG
      Serial.print(F("Found fx configuration ["));
      Serial.print(filename);
      Serial.println(F("]... loading..."));
#endif
      sysex = SD.open(filename);
      if (sysex)
      {
        if (!get_sd_data(sysex, 0x43, (uint8_t*)&configuration.fx, sizeof(configuration.fx)))
        {
          sysex.close();
          return false;
        }
        set_fx_params();
        sysex.close();

        return (true);
      }
#ifdef DEBUG
      else
      {
        Serial.print(F("E : Cannot open "));
        Serial.print(filename);
        Serial.println(F(" on SD."));
      }
#endif
    }
    else
    {
#ifdef DEBUG
      Serial.print(F("No "));
      Serial.print(filename);
      Serial.println(F(" available."));
#endif
    }
  }

  return (false);
}

bool save_sd_fx(uint8_t fx)
{
  fx = constrain(fx, 0, MAX_FX);

  if (sd_card > 0)
  {
    char filename[FILENAME_LEN];

    snprintf(filename, sizeof(filename), "/%s/%s%d.syx", FX_CONFIG_PATH, FX_CONFIG_NAME, fx);

#ifdef DEBUG
    Serial.print(F("Saving fx config "));
    Serial.print(fx);
    Serial.print(F(" to "));
    Serial.println(filename);
#endif

    return replace_sd_configuration(filename, 0x43, (uint8_t*)&configuration.fx, sizeof(configuration.fx));
  }

  return (false);
}

/******************************************************************************
   SD PERFORMANCE
 ******************************************************************************/
bool load_sd_performance(int8_t p)
{
  if (p < 0)
    return (false);

  p = constrain(p, 0, MAX_PERFORMANCE);

  if (sd_card > 0)
  {
    File sysex;
    char filename[FILENAME_LEN];

    sprintf(filename, "/%s/%s%d.syx", PERFORMANCE_CONFIG_PATH, PERFORMANCE_CONFIG_NAME, p);

    // first check if file exists...
    if (SD.exists(filename))
    {
      // ... and if: load
#ifdef DEBUG
      Serial.print(F("Found performance configuration ["));
      Serial.print(filename);
      Serial.println(F("]... loading..."));
#endif
      sysex = SD.open(filename);
      if (sysex)
      {
        if (!get_sd_data(sysex, 0x44, (uint8_t*)&configuration.performance, sizeof(configuration.performance)))
        {
          sysex.close();
          return false;
        }
        sysex.close();

        for (uint8_t instance_id = 0; instance_id < NUM_DEXED; instance_id++)
        {
          load_sd_voice(configuration.performance.bank[instance_id], configuration.performance.voice[instance_id], instance_id);
          load_sd_voiceconfig(configuration.performance.voiceconfig_number[instance_id], instance_id);

          MicroDexed[instance_id]->controllers.refresh();
          MicroDexed[instance_id]->panic();
        }
        load_sd_fx(configuration.performance.fx_number);

        return (true);
      }
#ifdef DEBUG
      else
      {
        Serial.print(F("E : Cannot open "));
        Serial.print(filename);
        Serial.println(F(" on SD."));
      }
#endif
    }
    else
    {
#ifdef DEBUG
      Serial.print(F("No "));
      Serial.print(filename);
      Serial.println(F(" available."));
#endif
    }
  }
  return (false);
}

bool save_sd_performance(uint8_t p)
{
  p = constrain(p, 0, MAX_PERFORMANCE);

  if (sd_card > 0)
  {
    char filename[FILENAME_LEN];

    snprintf(filename, sizeof(filename), "/%s/%s%d.syx", PERFORMANCE_CONFIG_PATH, PERFORMANCE_CONFIG_NAME, p);

#ifdef DEBUG
    Serial.print(F("Saving performance config "));
    Serial.print(p);
    Serial.print(F(" to "));
    Serial.println(filename);
#endif

    return replace_sd_configuration(filename, 0x44, (uint8_t*)&configuration.performance, sizeof(configuration.performance));
  }
  return (false);
}

/******************************************************************************
   HELPER FUNCTIONS
 ******************************************************************************/
bool get_sd_data(File sysex, uint8_t format, uint8_t* conf, uint16_t len)
{
  // Three header bytes, payload, checksum, and end byte.
  // Validate the complete payload before changing live configuration.
  if (sysex.size() != uint32_t(len) + 5 || !sysex.seek(0) ||
      sysex.read() != 0xf0 || sysex.read() != 0x67 || sysex.read() != format)
    return false;

  int32_t checksum = 0;
  for (uint16_t i = 0; i < len; ++i)
  {
    int value = sysex.read();
    if (value < 0)
      return false;
    checksum -= value;
  }
  if (sysex.read() != (checksum & 0x7f) || sysex.read() != 0xf7)
    return false;
  if (!sysex.seek(3))
    return false;
  return sysex.read(conf, len) == len;
}

bool write_sd_data(File sysex, uint8_t format, uint8_t* data, uint16_t len)
{
  if (!sysex.seek(0))
    return false;
  if (sysex.write(uint8_t(0xf0)) != 1 ||
      sysex.write(uint8_t(0x67)) != 1 ||
      sysex.write(format) != 1 || sysex.write(data, len) != len ||
      sysex.write(calc_checksum(data, len)) != 1 ||
      sysex.write(uint8_t(0xf7)) != 1)
    return false;
  sysex.flush();
  return sysex.size() == uint32_t(len) + 5;
}

bool replace_sd_configuration(const char* filename, uint8_t format, uint8_t* data, uint16_t len)
{
  char temporary[FILENAME_LEN + 5];
  char backup[FILENAME_LEN + 5];
  if (snprintf(temporary, sizeof(temporary), "%s.tmp", filename) >= int(sizeof(temporary)) ||
      snprintf(backup, sizeof(backup), "%s.bak", filename) >= int(sizeof(backup)))
    return false;
  // A retained backup may indicate an interrupted replacement. Preserve it.
  if (SD.exists(backup))
    return false;
  if (SD.exists(temporary) && !SD.remove(temporary))
    return false;
  File staged = SD.open(temporary, FILE_WRITE);
  if (!staged)
    return false;
  bool written = write_sd_data(staged, format, data, len);
  staged.close();
  if (!written)
  {
    SD.remove(temporary);
    return false;
  }
  return install_staged_sd_file(filename, temporary, backup);
}

bool install_staged_sd_file(const char* filename, const char* temporary, const char* backup)
{
  bool replacing = SD.exists(filename);
  if (replacing && !SD.rename(filename, backup))
  {
    SD.remove(temporary);
    return false;
  }
  if (!SD.rename(temporary, filename))
  {
    if (replacing)
      SD.rename(backup, filename);
    SD.remove(temporary);
    return false;
  }
  // If cleanup fails, report failure and retain the previous file as .bak.
  return !replacing || SD.remove(backup);
}

bool valid_dx7_bank(const uint8_t* data)
{
  if (data[0] != 0xf0 || data[1] != 0x43 || data[2] > 15 ||
      data[3] != 9 || data[4] != 32 || data[5] != 0 || data[4103] != 0xf7)
    return false;
  int32_t checksum = 0;
  for (uint16_t i = 6; i < 4102; ++i)
  {
    if (data[i] > 127)
      return false;
    checksum -= data[i];
  }
  return data[4102] == (checksum & 127);
}

bool replace_sd_bank_file(const char* filename, uint8_t* data)
{
  if (!valid_dx7_bank(data))
    return false;
  char temporary[FILENAME_LEN + 5];
  char backup[FILENAME_LEN + 5];
  if (snprintf(temporary, sizeof(temporary), "%s.tmp", filename) >= int(sizeof(temporary)) ||
      snprintf(backup, sizeof(backup), "%s.bak", filename) >= int(sizeof(backup)) ||
      SD.exists(backup))
    return false;
  if (SD.exists(temporary) && !SD.remove(temporary))
    return false;
  File staged = SD.open(temporary, FILE_WRITE);
  if (!staged)
    return false;
  bool written = staged.write(data, 4104) == 4104;
  staged.flush();
  written = written && staged.size() == 4104;
  staged.close();
  if (!written)
  {
    SD.remove(temporary);
    return false;
  }
  return install_staged_sd_file(filename, temporary, backup);
}

uint8_t calc_checksum(uint8_t* data, uint16_t len)
{
  int32_t bulk_checksum_calc = 0;

  for (uint16_t n = 0; n < len; n++)
    bulk_checksum_calc -= data[n];

  return (bulk_checksum_calc & 0x7f);
}

void strip_extension(const char* s, char* target, uint8_t len)
{
  if (len == 0)
    return;
  size_t count = strcspn(s, ".");
  count = min(count, size_t(len - 1));
  memcpy(target, s, count);
  target[count] = '\0';
}

bool get_bank_name(uint8_t b, char* name, uint8_t len)
{
  if (len == 0)
    return false;
  name[0] = '\0';
  if (sd_card <= 0 || b >= MAX_BANKS)
    return false;

  char bankdir[5];
  snprintf(bankdir, sizeof(bankdir), "/%u", unsigned(b));
  File directory = SD.open(bankdir);
  if (!directory || !directory.isDirectory())
    return false;

  bool found = false;
  while (true)
  {
    File entry = directory.openNextFile();
    if (!entry)
      break;
    const char* filename = entry.name();
    const char* extension = strrchr(filename, '.');
    size_t stem_length = extension ? size_t(extension - filename) : 0;
    if (!entry.isDirectory() && filename[0] != '.' && extension &&
        strcasecmp(extension, ".syx") == 0 && stem_length > 0 &&
        stem_length < BANK_NAME_LEN && stem_length < len && entry.size() == 4104)
    {
      // More than one usable bank is ambiguous; never select by directory order.
      if (found)
      {
        name[0] = '\0';
        entry.close();
        directory.close();
        return false;
      }
      memcpy(name, filename, stem_length);
      name[stem_length] = '\0';
      found = true;
    }
    entry.close();
  }
  directory.close();
  return found;
}

bool get_voice_name(uint8_t b, uint8_t v, char* name, uint8_t len)
{
  File sysex;

  if (sd_card > 0)
  {
    char bank_name[BANK_NAME_LEN];
    char filename[FILENAME_LEN];

    b = constrain(b, 0, MAX_BANKS - 1);
    v = constrain(v, 0, MAX_VOICES - 1);

    if (!get_bank_name(b, bank_name, sizeof(bank_name)))
      return false;
    snprintf(filename, sizeof(filename), "/%u/%s.syx", unsigned(b), bank_name);
#ifdef DEBUG
    Serial.print(F("Reading voice-name from ["));
    Serial.print(filename);
    Serial.println(F("]"));
#endif

    // try to open directory
    sysex = SD.open(filename);
    if (!sysex)
      return (false);

    memset(name, 0, len);
    sysex.seek(124 + (v * 128));
    sysex.read(name, min(len, 10));

#ifdef DEBUG
    Serial.print(F("Found voice-name ["));
    Serial.print(name);
    Serial.print(F("] for bank ["));
    Serial.print(b);
    Serial.print(F("] and voice ["));
    Serial.print(v);
    Serial.println(F("]"));
#endif

    sysex.close();

    return (true);
  }

  return (false);
}

bool get_voice_by_bank_name(uint8_t b, const char* bank_name, uint8_t v, char* voice_name, uint8_t len)
{
  File sysex;

  if (sd_card > 0)
  {
    char filename[FILENAME_LEN];

    sprintf(filename, "/%d/%s.syx", b, bank_name);
#ifdef DEBUG
    Serial.print(F("Reading voice-name from ["));
    Serial.print(filename);
    Serial.println(F("]"));
#endif

    // try to open directory
    sysex = SD.open(filename);
    if (!sysex)
      return (false);

    memset(voice_name, 0, len);
    sysex.seek(124 + (v * 128));
    sysex.read(voice_name, min(len, 10));

#ifdef DEBUG
    Serial.print(F("Found voice-name ["));
    Serial.print(voice_name);
    Serial.print(F("] for bank ["));
    Serial.print(b);
    Serial.print(F("|"));
    Serial.print(bank_name);
    Serial.print(F("] and voice ["));
    Serial.print(v);
    Serial.println(F("]"));
#endif

    sysex.close();

    return (true);
  }

  return (false);
}
