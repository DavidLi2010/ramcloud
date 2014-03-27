/* Copyright (c) 2014 Stanford University
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR(S) DISCLAIM ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL AUTHORS BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#include "Context.h"
#include "ObjectFinder.h"
#include "RamCloud.h"
#include "RpcWrapper.h"

#ifndef RAMCLOUD_INDEXRPCWRAPPER_H
#define RAMCLOUD_INDEXRPCWRAPPER_H

namespace RAMCloud {

// Forward declarations
class RamCloud;
class MasterService;

// TODO(ankitak): This class needs unit testing!

/**
 * IndexRpcWrapper manages the client side of RPCs that must be sent to
 * the server that store a particular the first index key of a key range.
 * If the server becomes unavailable or if it doesn't actually store the
 * desired key, then this class will retry the RPC with a different server
 * until it eventually succeeds.
 * 
 * RPCs using this wrapper will never fail to complete, though they may loop
 * forever. This wrapper is used for index lookup operation in RamCloud.
 * 
 * 
 */
class IndexRpcWrapper : public RpcWrapper {
  public:
    explicit IndexRpcWrapper(
            RamCloud* ramcloud, uint64_t tableId, uint8_t indexId,
            const void* key, uint16_t keyLength,
            uint32_t responseHeaderLength, Buffer* responseBuffer);

    explicit IndexRpcWrapper(
            MasterService* master, uint64_t tableId, uint8_t indexId,
            const void* key, uint16_t keyLength,
            uint32_t responseHeaderLength);

    /**
     * Destructor for IndexRpcWrapper.
     */
    virtual ~IndexRpcWrapper() {}

  PROTECTED:
    virtual bool checkStatus();
    virtual bool handleTransportError();
    virtual void send();

    /// Overall information about the calling process.
    Context* context;

    /// ObjectFinder corresponding to either the client or the server
    /// invoking this rpc; used to find the server to send the rpc to.
    ObjectFinder* objectFinder;

    /// Information about next key that determines which server the request
    /// is sent to; we have to save this information for use in retries.
    uint64_t tableId;
    uint8_t indexId;
    const void* key;
    uint16_t keyLength;
    Buffer* responseBuffer;

    DISALLOW_COPY_AND_ASSIGN(IndexRpcWrapper);
};

} // end RAMCloud

#endif  // RAMCLOUD_INDEXRPCWRAPPER_H
