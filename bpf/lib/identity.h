/*
 *  Copyright (C) 2016-2020 Authors of Cilium
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */
#ifndef __LIB_IDENTITY_H_
#define __LIB_IDENTITY_H_

#include "drop.h"
#include "dbg.h"

/**
 * identity_is_reserved is used to determine whether an identity is one of the
 * reserved identities that are not handed out to endpoints.
 *
 * Specifically, it should return true if the identity is one of these:
 * - IdentityUnknown
 * - ReservedIdentityHost
 * - ReservedIdentityWorld
 * - ReservedIdentityRemoteNode
 *
 * The following identities are given to endpoints so return false for these:
 * - ReservedIdentityUnmanaged
 * - ReservedIdentityHealth
 * - ReservedIdentityInit
 *
 * Identities 128 and higher are guaranteed to be generated based on user input.
 */
static inline bool identity_is_reserved(__u32 identity)
{
	return identity < UNMANAGED_ID || identity == REMOTE_NODE_ID;
}

static inline bool __inline__ inherit_identity_from_host(struct __sk_buff *skb, __u32 *identity)
{
	__u32 magic = skb->mark & MARK_MAGIC_HOST_MASK;
	bool from_proxy = false;

	/* Packets from the ingress proxy must skip the proxy when the
	 * destination endpoint evaluates the policy. As the packet
	 * would loop and/or the connection be reset otherwise. */
	if (magic == MARK_MAGIC_PROXY_INGRESS) {
		*identity = get_identity(skb);
		skb->tc_index |= TC_INDEX_F_SKIP_INGRESS_PROXY;
		from_proxy = true;
	/* (Return) packets from the egress proxy must skip the
	 * redirection to the proxy, as the packet would loop and/or
	 * the connection be reset otherwise. */
	} else if (magic == MARK_MAGIC_PROXY_EGRESS) {
		*identity = get_identity(skb);
		skb->tc_index |= TC_INDEX_F_SKIP_EGRESS_PROXY;
		from_proxy = true;
	} else if (magic == MARK_MAGIC_IDENTITY) {
		*identity = get_identity(skb);
	} else if (magic == MARK_MAGIC_HOST) {
		*identity = HOST_ID;
	} else {
		*identity = WORLD_ID;
	}

	/* Reset packet mark to avoid hitting routing rules again */
	skb->mark = 0;
	cilium_dbg(skb, DBG_INHERIT_IDENTITY, *identity, 0);

	return from_proxy;
}

#endif