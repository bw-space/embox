/**
 * @file
 * @brief Realization of the eXternal Data Representation
 * @details XDR: External Data Representation Standard, RFC 1832
 * @date 19.05.12
 * @author Ilia Vaprol
 */

#ifndef NET_RPC_XDR_H_
#define NET_RPC_XDR_H_

#include <stddef.h>
#include <types.h>

/* Prototypes */
struct xdr;

/* Standard size of XDR unit is 4 bytes */
#define BYTES_PER_XDR_UNIT 4
typedef __u32 xdr_unit_t;

#define XDR_SUCCESS     1
#define XDR_FAILURE     0
#define XDR_LAST_UINT32 ((__u32)-1)

enum xdr_op {
	XDR_ENCODE,
	XDR_DECODE,
	XDR_FREE
};

union xdrrec_hdr {
	struct {
		__u32 len:31,
			is_last:1;
	} h;
	xdr_unit_t unit;
};

typedef int (*xdrproc_t)(struct xdr *, void *, ...);
typedef int (*xdrrec_hnd_t)(char *, char *, int);

struct xdr_ops {
	int (*getunit)(struct xdr *xs, xdr_unit_t *to);
	int (*putunit)(struct xdr *xs, const xdr_unit_t *from);
	int (*getbytes)(struct xdr *xs, char *to, size_t size);
	int (*putbytes)(struct xdr *xs, const char *from, size_t size);
	size_t (*getpos)(struct xdr *xs);
	int (*setpos)(struct xdr *xs, size_t pos);
	void (*destroy)(struct xdr *xs);
};

struct xdr {
	enum xdr_op oper;
	const struct xdr_ops *ops;
	/* Specific options */
	union {
		struct {
			char *buff;
			char *curr;
			size_t left;
		} mem;
		struct {
			char *handle;
			/* in-coming */
			xdrrec_hnd_t in_hnd;
			char *in_base;
			char *in_hdr;
			char *in_curr;
			char *in_boundry;
			size_t in_prep;
			size_t in_left;
			char in_last;
			/* out-going */
			xdrrec_hnd_t out_hnd;
			char *out_base;
			char *out_hdr;
			char *out_curr;
			char *out_boundry;
		} rec;
	} extra;
};

struct xdr_discrim {
	int value;
	xdrproc_t proc;
};

extern void xdrmem_create(struct xdr *xs, char *addr, size_t size, enum xdr_op oper);
extern void xdrrec_create(struct xdr *xs, unsigned int sendsz, unsigned int recvsz,
		char *handle, xdrrec_hnd_t readit, xdrrec_hnd_t writeit);
extern int xdrrec_endofrecord(struct xdr *xs, int sendnow);
extern size_t xdr_getpos(struct xdr *xs);
extern int xdr_setpos(struct xdr *xs, size_t pos);
extern void xdr_destroy(struct xdr *xs);
extern void xdr_free(xdrproc_t proc, char *obj);

/* XDR filters */
extern int xdr_void(void);
extern int xdr_int(struct xdr *xs, __s32 *pi);
extern int xdr_u_int(struct xdr *xs, __u32 *pui);
extern int xdr_short(struct xdr *xs, __s16 *ps);
extern int xdr_u_short(struct xdr *xs, __u16 *pus);
extern int xdr_enum(struct xdr *xs, __s32 *pe);
extern int xdr_bool(struct xdr *xs, __s32 *pb);
extern int xdr_bytes(struct xdr *xs, char **ppc, __u32 *psize, __u32 maxsize);
extern int xdr_opaque(struct xdr *xs, char *pc, size_t size);
extern int xdr_string(struct xdr *xs, char **pstr, __u32 maxsize);
extern int xdr_wrapstring(struct xdr *xs, char **pstr);
extern int xdr_union(struct xdr *xs, __s32 *pdiscriminant, void *punion,
		const struct xdr_discrim *choices, xdrproc_t dfault);

#endif /* NET_RPC_XDR_H_ */
