#ifndef PCI_ST_H
#define PCI_ST_H

/*
 * There is one pci_dev structure for each slot-number/function-number
 * combination:
 */
struct pci_dev {
	struct pci_bus	*bus;		/* bus this device is on */
	struct pci_dev	*sibling;	/* next device on this bus */
	struct pci_dev	*next;		/* chain of all devices */

	void		*sysdata;	/* hook for sys-specific extension */

	unsigned int	devfn;		/* encoded device & function index */
	unsigned short	vendor;
	unsigned short	device;
	unsigned int	class;		/* 3 bytes: (base,sub,prog-if) */
	unsigned int	master : 1;	/* set if device is master capable */
	/*
	 * In theory, the irq level can be read from configuration
	 * space and all would be fine.  However, old PCI chips don't
	 * support these registers and return 0 instead.  For example,
	 * the Vision864-P rev 0 chip can uses INTA, but returns 0 in
	 * the interrupt line and pin registers.  pci_init()
	 * initializes this field with the value at PCI_INTERRUPT_LINE
	 * and it is the job of pcibios_fixup() to change it if
	 * necessary.  The field must not be 0 unless the device
	 * cannot generate interrupts at all.
	 */
	unsigned char	irq;		/* irq generated by this device */
};

struct pci_bus {
	struct pci_bus	*parent;	/* parent bus this bridge is on */
	struct pci_bus	*children;	/* chain of P2P bridges on this bus */
	struct pci_bus	*next;		/* chain of all PCI buses */

	struct pci_dev	*self;		/* bridge device as seen by parent */
	struct pci_dev	*devices;	/* devices behind this bridge */

	void		*sysdata;	/* hook for sys-specific extension */

	unsigned char	number;		/* bus number */
	unsigned char	primary;	/* number of primary bridge */
	unsigned char	secondary;	/* number of secondary bridge */
	unsigned char	subordinate;	/* max number of subordinate buses */
};

/*
 * This is used to map a vendor-id/device-id pair into device-specific
 * information.
 */
struct pci_dev_info {
	unsigned short	vendor;		/* vendor id */
	unsigned short	device;		/* device id */

	const char	*name;		/* device name */
	unsigned char	bridge_type;	/* bridge type or 0xff */
};

/* Used on intel */
struct indirect_t {
    unsigned long address;
    unsigned short segment;
};

/*
 * All of the stae required by the PCI module
 */
struct pci_st {
    struct pci_bus  pci_root;
    struct pci_dev *pci_devices;
    unsigned int    io_base;  /* io_base for layout routines */    
    unsigned int    mem_base; /* <16MB is ISA memory */
    unsigned long bios32_entry;
    long pcibios_entry; 
    struct indirect_t bios32_indirect;
    struct indirect_t pci_indirect;
};

/* 
 * Internal functions 
 */
extern unsigned long pci_init (struct pci_st *st,
			       unsigned long mem_start, 
			       unsigned long mem_end);

extern struct pci_dev_info *pci_lookup_dev (unsigned int vendor,
					    unsigned int dev);
extern const char *pci_strclass (unsigned int class);
extern const char *pci_strvendor (unsigned int vendor);
extern const char *pci_strdev (unsigned int vendor, unsigned int device);

extern int get_pci_list (struct pci_st *st, char *buf);

#endif /* PCI_H */