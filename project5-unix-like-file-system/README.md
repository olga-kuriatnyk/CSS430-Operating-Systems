#### BFS  - a Unix-Like FileSystem

The filesystem comprises 3 layers.  From top to bottom, these are:

* fs – user-level filesystem, with functions like fsOpen, fsRead, fsSeek, fsClose.

* bfs – functions internal to BFS such as bfsFindFreeBlock, bfsInitFreeList.

* bio - lowest level block IO functions: bioRead and bioWrite.
