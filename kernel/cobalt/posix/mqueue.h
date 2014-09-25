/*
 * Written by Gilles Chanteperdrix <gilles.chanteperdrix@xenomai.org>.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

#ifndef _COBALT_POSIX_MQUEUE_H
#define _COBALT_POSIX_MQUEUE_H

#include <linux/types.h>
#include <linux/fcntl.h>
#include <xenomai/posix/syscall.h>

struct mq_attr {
	long mq_flags;
	long mq_maxmsg;
	long mq_msgsize;
	long mq_curmsgs;
};

COBALT_SYSCALL_DECL(mq_open,
		    int, (const char __user *u_name, int oflags,
			  mode_t mode, struct mq_attr __user *u_attr));

COBALT_SYSCALL_DECL(mq_close, int, (mqd_t uqd));

COBALT_SYSCALL_DECL(mq_unlink,
		    int, (const char __user *u_name));

COBALT_SYSCALL_DECL(mq_getattr, 
		    int, (mqd_t uqd, struct mq_attr __user *u_attr));

COBALT_SYSCALL_DECL(mq_setattr,
		    int, (mqd_t uqd, const struct mq_attr __user *u_attr,
			  struct mq_attr __user *u_oattr));

COBALT_SYSCALL_DECL(mq_timedsend,
		    int, (mqd_t uqd, const void __user *u_buf, size_t len,
			  unsigned int prio,
			  const struct timespec __user *u_ts));

COBALT_SYSCALL_DECL(mq_timedreceive,
		    int, (mqd_t uqd, void __user *u_buf,
			 ssize_t __user *u_len,
			 unsigned int __user *u_prio,
			 const struct timespec __user *u_ts));

COBALT_SYSCALL_DECL(mq_notify,
		    int, (mqd_t fd, const struct sigevent *__user evp));

int cobalt_mq_pkg_init(void);

void cobalt_mq_pkg_cleanup(void);

#endif /* !_COBALT_POSIX_MQUEUE_H */