// Copyright (C) 2016 iNuron NV
//
// This file is part of Open vStorage Open Source Edition (OSE),
// as available from
//
//      http://www.openvstorage.org and
//      http://www.openvstorage.com.
//
// This file is free software; you can redistribute it and/or modify it
// under the terms of the GNU Affero General Public License v3 (GNU AGPLv3)
// as published by the Free Software Foundation, in version 3 as it comes in
// the LICENSE.txt file of the Open vStorage OSE distribution.
// Open vStorage is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY of any kind.

#ifndef VFS_AMQP_URI_H_
#define VFS_AMQP_URI_H_

#include <youtils/InitializedParam.h>
#include <youtils/StrongTypedString.h>

STRONG_TYPED_STRING(volumedriverfs, AmqpUri);
STRONG_TYPED_STRING(volumedriverfs, AmqpExchange);
STRONG_TYPED_STRING(volumedriverfs, AmqpRoutingKey);

namespace volumedriverfs
{

using AmqpUris = std::vector<AmqpUri>;

std::ostream&
operator<<(std::ostream& os,
           const AmqpUris& uris);

}

namespace initialized_params
{

template<>
struct PropertyTreeVectorAccessor<volumedriverfs::AmqpUri>
{
    using Type = volumedriverfs::AmqpUri;

    static const std::string key;

    static Type
    get(const boost::property_tree::ptree& pt)
    {
        return Type(pt.get<std::string>(key));
    }

    static void
    put(boost::property_tree::ptree& pt,
        const Type& uri)
    {
        pt.put(key, uri);
    }
};

}

#endif
