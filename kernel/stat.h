#define T_DIR     1   // Directory
#define T_FILE    2   // File
#define T_DEVICE  3   // Device

struct stat {
  int dev;     // File system's disk device 文件系统的磁盘设备
  uint ino;    // Inode number  Inode编号
  short type;  // Type of file  文件类型
  short nlink; // Number of links to file  指向文件的链接数
  uint64 size; // Size of file in bytes  文件字节数
};
