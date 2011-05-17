/*
This work is licensed under the Creative Commons Attribution 3.0 Unported License.
To view a copy of this license, visit http://creativecommons.org/licenses/by/3.0/
or send a letter to
        Creative Commons,
        171 Second Street,
        Suite 300,
        San Francisco,
        California,
        94105,
        USA.
 */

#ifndef __USB_STACK_H__
#define __USB_STACK_H__

// JTR v0.1a

#include "PICUSB.h"     // JTR combined PIC_18F and PIC_24F header. This is the only included header.

#define USB_TOKEN_Mask  0x3c
#define USB_TOKEN_OUT   0x04
#define USB_TOKEN_IN    0x24
#define USB_TOKEN_SOF   0x14
#define USB_TOKEN_SETUP 0x34
#define USB_TOKEN_DATA0 0x0c
#define USB_TOKEN_DATA1 0x2c
#define USB_TOKEN_DATA2 0x1c              /* High speed isochronous endpoints only */
#define USB_TOKEN_MDATA 0x3c              /* High speed isochronous enpoints and hub devices only */
#define USB_TOKEN_ACK   0x08
#define USB_TOKEN_NAK   0x28
#define USB_TOKEN_STALL 0x38
#define USB_TOKEN_NYET  0x18              /* High speed devices only */
#define USB_TOKEN_PRE   0x30
#define USB_TOKEN_ERR   0x30
#define USB_TOKEN_SPLIT 0x20              /* Hub devices only */
#define USB_TOKEN_PING  0x10              /* High speed devices only */

/* Descriptor Types */
#define USB_DEVICE_DESCRIPTOR_TYPE                      1u
#define USB_CONFIGURATION_DESCRIPTOR_TYPE               2u
#define USB_STRING_DESCRIPTOR_TYPE                      3u
#define USB_INTERFACE_DESCRIPTOR_TYPE                   4u
#define USB_ENDPOINT_DESCRIPTOR_TYPE                    5u
#define USB_DEVICE_QUALIFIER_DESCRIPTOR_TYPE            6u
#define USB_OTHER_SPEED_CONFIGURATION_DESCRIPTOR_TYPE   7u
#define USB_INTERFACE_POWER_DESCRIPTOR_TYPE             8u
#define USB_OTG_DESCRIPTOR_TYPE                         9u
#define USB_DEBUG_DESCRIPTOR_TYPE                       10u
#define USB_INTERFACE_ASSOCIATION_DESCRIPTOR_TYPE       11u

#define USB_bmRequestType                       0
#define USB_bRequest                            1
#define USB_wValue                              2
#define USB_bDescriptorIndex                    2
#define USB_wValueHigh                          3
#define USB_bDescriptorType                     3
#define USB_wIndex                              4
#define USB_bInterface                          4
#define USB_wIndexHigh                          5
#define USB_wLength                             6
#define USB_wLengthHigh                         7

#define USB_bmRequestType_PhaseMask             0x80
#define USB_bmRequestType_H2D                   0x00
#define USB_bmRequestType_D2H                   0x80
#define USB_bmRequestType_TypeMask              0x60
#define USB_bmRequestType_Standard              0x00
#define USB_bmRequestType_Class                 0x20
#define USB_bmRequestType_Vendor                0x40
#define USB_bmRequestType_RecipientMask         0x03
#define USB_bmRequestType_Device                0x00
#define USB_bmRequestType_Interface             0x01
#define USB_bmRequestType_Endpoint              0x02
#define USB_bmRequestType_Other                 0x03

#define USB_REQUEST_GET_STATUS                  0
#define USB_REQUEST_CLEAR_FEATURE               1
#define USB_REQUEST_SET_FEATURE                 3
#define USB_REQUEST_SET_ADDRESS                 5
#define USB_REQUEST_GET_DESCRIPTOR              6
#define USB_REQUEST_SET_DESCRIPTOR              7
#define USB_REQUEST_GET_CONFIGURATION           8
#define USB_REQUEST_SET_CONFIGURATION           9
#define USB_REQUEST_GET_INTERFACE               10
#define USB_REQUEST_SET_INTERFACE               11
#define USB_REQUEST_SYNCH_FRAME                 12


//  #include "usb_lang.h" JTR only required by descriptors.h
/* Include user configuration */
// #include "usb_config.h"


// JTR Moved to descriptor.h as you cannot have a descriptor without a VID_PID pair.
//#ifndef USB_VID
//#error "Must supply own Vendor ID (USB_VID) in usb_config.h"
//#endif
//#ifndef USB_PID
//#error "Must supply Product ID (USB_PID) in usb_config.h"
//#endif

// JTR PICUSB header file replaces these. 
// #if defined(PIC_18F)
// #include "usb_p18f.h"
// #elif defined(PIC_24F) 
// #include "usb_p24f.h"
// #endif

/* Structs for defining endpoints */
//typedef void(*)(unsigned char *) usb_ep_callback;
typedef void(*usb_handler_t)(void);

typedef struct USB_EP_TYPE {
//    usb_uep_t type;
//    unsigned int buffer_size;
//    unsigned char *in_buffer, *out_buffer;
    usb_handler_t in_handler, out_handler;
} usb_ep_t;

/* Misc */
#define HIGHB(x) ((x)>>8)
#define LOWB(x) ((x) & 0xFF)

#define XCAT(x,y) x ## y
#define CAT(x,y) XCAT(x,y)

/* Descriptors */
#if USB_NUM_CONFIGURATIONS > 1
#error "More than 1 configuration not supported yet"
#endif
/*
#if defined class_init
 #ifndef class_setup
  #error "No Class setup handler defined"
 #endif
#elif defined vendor_init
 #ifndef vendor_setup
  #error "No Vendor setup handler defined"
 #endif
#else
 #error "Niether Class nor Vendor initialization function defined"
#endif
 */

/* JTR I moved this block into PICUSB.h
 * My rational is to keep this header free from
 * conditional compile directives based on what
 * PIC family is being compiled for. It just helps
 * to keep things orderly and makes for less work
 * when someone adds support for the new PIC24E parts.


#if defined(PIC_18F)
typedef struct BDENTRY {
        unsigned char
//      struct {
//                unsigned BCH:2;
//                unsigned BSTALL:1;
//                unsigned DTSEN:1;
//                unsigned INCDIS:1;
//                unsigned KEN:1;
//                unsigned DTS:1;
//                unsigned UOWN:1;
//        }
        BDSTAT;
        unsigned char BDCNT;
        unsigned char *BDADDR;
} BDentry;
#else
typedef struct BDENTRY {
        unsigned char BDCNT;    // JTR PIC24 fixup Note that BDCNT & BDSTAT are swapped from the PIC18!!
        unsigned char BDSTAT;   // Smacks head. Go on, yo know you want to. 
        unsigned char *BDADDR;
} BDentry;
#endif
 */

/* BDSTAT Bitmasks */

// JTR moved to PICUSB.h Common to both PIC18 and PIC24FJ
//#define UOWN    0x80
//#define DTS     0x40
//#define KEN     0x20
//#define INCDIS  0x10
//#define DTSEN   0x08
//#define BSTALL  0x04
//#define BC98    0x03

extern BDentry usb_bdt[];

// JTR move EP0 size check to usb_stack.c. see further comments there.

typedef struct USB_DEVICE_REQUEST {
    unsigned char bmRequestType;
    unsigned char bRequest;
    unsigned int wValue;
    unsigned int wIndex;
    unsigned int wLength;
} usb_device_request;

typedef unsigned char usb_status_t;
extern usb_status_t trn_status;
extern BDentry *bdp, *rbdp;

extern void usb_init(ROMPTR const unsigned char *dev_descriptor,
        ROMPTR const unsigned char *config_descriptor,
        ROMPTR const unsigned char *string_descriptor, int num_string_descriptors);


extern void usb_start(void);
extern void usb_register_sof_handler(usb_handler_t handler);
extern void usb_register_class_setup_handler(usb_handler_t handler);
extern void usb_register_vendor_setup_handler(usb_handler_t handler);
//extern void usb_register_endpoint(unsigned int endpoint, usb_uep_t type,
//        unsigned int buffer_size, unsigned char *out_buffer, unsigned char *in_buffer,
//        usb_handler_t out_handler, usb_handler_t in_handler);
extern void usb_set_in_handler(int ep, usb_handler_t handler);
extern void usb_set_out_handler(int ep, usb_handler_t handler);
#define usb_unset_in_handler(ep) usb_set_in_handler(ep, (usb_handler_t) 0)
#define usb_unset_out_handler(ep) usb_set_out_handler(ep, (usb_handler_t) 0)    // I will friggin die if this turns out to be the PIC18 bug!

extern void usb_handler(void);

extern void usb_ack(BDentry *);
extern void usb_ack_zero(BDentry *);
// JTR N/A extern void usb_ack_out(BDentry *);
extern void usb_RequestError(void);

#endif /* USB_STACK_H */