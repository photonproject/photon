// Copyright (c) 2009-2012 The Bitcoin developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <boost/assign/list_of.hpp> // for 'map_list_of()'
#include <boost/foreach.hpp>

#include "checkpoints.h"

#include "main.h"
#include "uint256.h"

namespace Checkpoints
{
    typedef std::map<int, uint256> MapCheckpoints;

    // How many times we expect transactions after the last checkpoint to
    // be slower. This number is a compromise, as it can't be accurate for
    // every system. When reindexing from a fast disk with a slow CPU, it
    // can be up to 20, while when downloading from a slow network with a
    // fast multicore CPU, it won't be much higher than 1.
    static const double fSigcheckVerificationFactor = 5.0;

    struct CCheckpointData {
        const MapCheckpoints *mapCheckpoints;
        int64 nTimeLastCheckpoint;
        int64 nTransactionsLastCheckpoint;
        double fTransactionsPerDay;
    };

    // What makes a good checkpoint block?
    // + Is surrounded by blocks with reasonable timestamps
    //   (no blocks before with a timestamp after, none after with
    //    timestamp before)
    // + Contains no strange transactions
    static MapCheckpoints mapCheckpoints =
        boost::assign::map_list_of
        ( 0, uint256("0x000000e79a20d718a2f2d8b98161dc6700104a22d8e9be70e8ac361ee6664b9c"))
        ( 4684, uint256("0x000000000000abb156fb3ba96155f880a8bd41ab3ca744cf45c91c51c7a7c42d"))
	( 5050, uint256("0x0000000000040709146699c80907b2aae30bb37c4ca99c21c9efe04f82966959"))
	( 15200, uint256("0x0000000000187d893af37ebcbac6aafa9483291af9248e3f4976c38606950811"))
	( 27205, uint256("0x00000000001b387301466a4a91aebc2c037556b8a76dcac227ca2796873b08e2"))
        ( 31300, uint256("0x00000000000668d829d8f12684ff0c63b6e7f84bdff988f7ef5831c116fe8615"))
        ( 49009, uint256("0x05e6756348da0e27b1547f530160c1b7aff1955751b3dbe1cef80ee715f0bd0b"))
        ( 75020, uint256("0x07676917780a8b0054f5c0f7006b5d3037f6d68b40af63e4c119e48d96ee90c6"))
        ( 101017, uint256("0x04a75ebd63e3e6cf4ade7958d8978bfaab7d905b081d337aa556b113c0c5c89d"))
	( 143027, uint256("0x0f9beea1816ff9053a15433373ecce12694ed8521d752d7205c33d8619f6a15e"))
        ( 180023, uint256("0x0b98a2b2358aab5dad42f1d8271da92c3624f00dc6af8ed996f7fae6991a92d2"))
        ( 220000, uint256("0x82640e0dcc90320fea369a3c86dff88173047f309cceb48898eeec46d577917a"))
        ( 239000, uint256("0x228019408d89f524736fa2656ad41979be9f0918b4412415f6f1e704e33e435f"))
        ( 297000, uint256("0xc189ea7f602a4fea8012e35befe137bbd653630868133c7336c2d89501abcc07"))
        
        ;
    static const CCheckpointData data = {
        &mapCheckpoints,
        1435666994, // * UNIX timestamp of last checkpoint block
        755296,     // * total number of transactions between genesis and last checkpoint
                    //   (the tx=... number in the SetBestChain debug.log lines)
        3000.0     // * estimated number of transactions per day after checkpoint
    };

    static MapCheckpoints mapCheckpointsTestnet = 
        boost::assign::map_list_of
        ( 0, uint256("0x00000052d978f26d698e0c4dbce9f8139a69f2fbda37715149146776aeb70d5b"))
        ;
    static const CCheckpointData dataTestnet = {
        &mapCheckpointsTestnet,
        1392351202,
        0,
        60000.0
    };

    const CCheckpointData &Checkpoints() {
        if (fTestNet)
            return dataTestnet;
        else
            return data;
    }

    bool CheckBlock(int nHeight, const uint256& hash)
    {
        if (!GetBoolArg("-checkpoints", true))
            return true;

        const MapCheckpoints& checkpoints = *Checkpoints().mapCheckpoints;

        MapCheckpoints::const_iterator i = checkpoints.find(nHeight);
        if (i == checkpoints.end()) return true;
        return hash == i->second;
    }

    // Guess how far we are in the verification process at the given block index
    double GuessVerificationProgress(CBlockIndex *pindex) {
        if (pindex==NULL)
            return 0.0;

        int64 nNow = time(NULL);

        double fWorkBefore = 0.0; // Amount of work done before pindex
        double fWorkAfter = 0.0;  // Amount of work left after pindex (estimated)
        // Work is defined as: 1.0 per transaction before the last checkoint, and
        // fSigcheckVerificationFactor per transaction after.

        const CCheckpointData &data = Checkpoints();

        if (pindex->nChainTx <= data.nTransactionsLastCheckpoint) {
            double nCheapBefore = pindex->nChainTx;
            double nCheapAfter = data.nTransactionsLastCheckpoint - pindex->nChainTx;
            double nExpensiveAfter = (nNow - data.nTimeLastCheckpoint)/86400.0*data.fTransactionsPerDay;
            fWorkBefore = nCheapBefore;
            fWorkAfter = nCheapAfter + nExpensiveAfter*fSigcheckVerificationFactor;
        } else {
            double nCheapBefore = data.nTransactionsLastCheckpoint;
            double nExpensiveBefore = pindex->nChainTx - data.nTransactionsLastCheckpoint;
            double nExpensiveAfter = (nNow - pindex->nTime)/86400.0*data.fTransactionsPerDay;
            fWorkBefore = nCheapBefore + nExpensiveBefore*fSigcheckVerificationFactor;
            fWorkAfter = nExpensiveAfter*fSigcheckVerificationFactor;
        }

        return fWorkBefore / (fWorkBefore + fWorkAfter);
    }

    int GetTotalBlocksEstimate()
    {
        if (!GetBoolArg("-checkpoints", true))
            return 0;

        const MapCheckpoints& checkpoints = *Checkpoints().mapCheckpoints;

        return checkpoints.rbegin()->first;
    }

    CBlockIndex* GetLastCheckpoint(const std::map<uint256, CBlockIndex*>& mapBlockIndex)
    {
        if (!GetBoolArg("-checkpoints", true))
            return NULL;

        const MapCheckpoints& checkpoints = *Checkpoints().mapCheckpoints;

        BOOST_REVERSE_FOREACH(const MapCheckpoints::value_type& i, checkpoints)
        {
            const uint256& hash = i.second;
            std::map<uint256, CBlockIndex*>::const_iterator t = mapBlockIndex.find(hash);
            if (t != mapBlockIndex.end())
                return t->second;
        }
        return NULL;
    }
}
