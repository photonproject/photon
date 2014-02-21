Forked from Bitcoin reference wallet 0.8.5 and Blake Coin

Photon is a cryptocurrency designed to use the Blake 256 algorithm cloned from Blake Coin.

Blake-256(optimized) faster than scrypt and faster than sha256 in C (asm is still faster for sha256d)

The algorithm was written as a candidate for sha3, Based on round one candidate code from the sphlib 2.1 and reduced the compression function to 8.

Tweaks Removed some of the double hashing from the wallet as it is wasteful on compute, No changes to the ecdsa public/private function as that has proven to be secure so far on bitcoin.


What is Blakecoin?

Blakecoin is an experimental new digital currency that enables instant payments to
anyone, anywhere in the world. Blakecoin uses peer-to-peer technology to operate
with no central authority: managing transactions and issuing of coins are carried
out collectively by the network.

What is Photon ?

A clone of Blakecoin with a few changes.
The standard block award of Photon is 32,768 BUT as the block chain grows the award will increase.
The amount it increases is directly related to the current difficulty and height of the blockchain.
Miners will get 32,768 coins plus the square root of blockchain height multiplied by the current difficulty.
Still as Photon's are plentiful in the universe the max money is set to 90,000,000,000
That is 90 Billion Photons. Difficulty retargets every 20 blocks with a target of a new block to be produced every 3 minutes.
Up to block 3500 the max adjustment is 15% up each retarget.
After block 3500 the max adjustment is 3% up each retarget.  

Ubuntu 12.04 dependancies that are used on the Linux build machine:

git-core build-essential libssl-dev libboost-all-dev libdb5.1-dev libdb5.1++-dev libgtk2.0-dev libminiupnpc-dev qt4-qmake mingw32 synaptic qt-sdk qt4-dev-tools libqt4-dev libqt4-core libqt4-gui libdb++-dev

License

Photon is released under the terms of the MIT license. See `COPYING` for more
information or see http://opensource.org/licenses/MIT.


Blakecoin is released under the terms of the MIT license. See `COPYING` for more
information or see http://opensource.org/licenses/MIT.



