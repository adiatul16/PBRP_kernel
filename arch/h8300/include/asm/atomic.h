#ifndef __ARCH_H8300_ATOMIC__
#define __ARCH_H8300_ATOMIC__

#include <linux/types.h>
#include <asm/cmpxchg.h>

/*
 * Atomic operations that C can't guarantee us.  Useful for
 * resource counting etc..
 */

#define ATOMIC_INIT(i)	{ (i) }

#define atomic_read(v)		READ_ONCE((v)->counter)
#define atomic_set(v, i)	WRITE_ONCE(((v)->counter), (i))

#include <linux/kernel.h>

#define ATOMIC_OP_RETURN(op, c_op)				\
static inline int atomic_##op##_return(int i, atomic_t *v)	\
{								\
	h8300flags flags;					\
	int ret;						\
								\
	flags = arch_local_irq_save();				\
	ret = v->counter c_op i;				\
	arch_local_irq_restore(flags);				\
	return ret;						\
}

#define ATOMIC_OP(op, c_op)					\
static inline void atomic_##op(int i, atomic_t *v)		\
{								\
	h8300flags flags;					\
								\
	flags = arch_local_irq_save();				\
	v->counter c_op i;					\
	arch_local_irq_restore(flags);				\
}

ATOMIC_OP_RETURN(add, +=)
ATOMIC_OP_RETURN(sub, -=)

ATOMIC_OP(and, &=)
ATOMIC_OP(or,  |=)
ATOMIC_OP(xor, ^=)

#undef ATOMIC_OP_RETURN
#undef ATOMIC_OP

#define atomic_add(i, v)		(void)atomic_add_return(i, v)
#define atomic_add_negative(a, v)	(atomic_add_return((a), (v)) < 0)

#define atomic_sub(i, v)		(void)atomic_sub_return(i, v)
#define atomic_sub_and_test(i, v)	(atomic_sub_return(i, v) == 0)

#define atomic_inc_return(v)		atomic_add_return(1, v)
#define atomic_dec_return(v)		atomic_sub_return(1, v)

#define atomic_inc(v)			(void)atomic_inc_return(v)
#define atomic_inc_and_test(v)		(atomic_inc_return(v) == 0)

#define atomic_dec(v)			(void)atomic_dec_return(v)
#define atomic_dec_and_test(v)		(atomic_dec_return(v) == 0)

static inline int atomic_cmpxchg(atomic_t *v, int old, int new)
{
	int ret;
	h8300flags flags;

	flags = arch_local_irq_save();
	ret = v->counter;
	if (likely(ret == old))
		v->counter = new;
	arch_local_irq_restore(flags);
	return ret;
}

static inline int atomic_fetch_add_unless(atomic_t *v, int a, int u)
{
	int ret;
	h8300flags flags;

	flags = arch_local_irq_save();
	ret = v->counter;
	if (ret != u)
		v->counter += a;
	arch_local_irq_restore(flags);
	return ret;
}
#define atomic_fetch_add_unless		atomic_fetch_add_unless

#endif /* __ARCH_H8300_ATOMIC __ */
