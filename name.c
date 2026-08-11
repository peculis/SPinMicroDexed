#include "usb_names.h"
#define USB_PRODUCT_NAME   {'S','P','i','n','M','i','c','r','o','D','e','x','e','d'}
#define USB_PRODUCT_NAME_LEN 14

// Do not change this part.  This exact format is required by USB.

struct usb_string_descriptor_struct usb_string_product_name = {
  2 + USB_PRODUCT_NAME_LEN * 2,
  3,
  USB_PRODUCT_NAME
};
