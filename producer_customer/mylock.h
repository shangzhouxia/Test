int lock_set(int fd, int type)
{
    struct flock old_lock, lock;

    lock.l_whence = SEEK_SET;
    lock.l_start = 0;
    lock.l_len = 0;
    lock.l_type = type;
    lock.l_pid = -1;


    //判断文件是否可以上锁
    fcntl(fd, F_GETLK, &lock);
    if (lock.l_type != F_UNLCK) {
        if (lock.l_type == F_RDLCK) {
            printf("RD lock already set by %d\n", lock.l_pid);
        } else if (lock.l_type == F_WRLCK) {
            printf("WR lock already set by %d\n", lock.l_pid);
        }
    }

    lock.l_type = type;

    if ( (fcntl(fd, F_SETLK, &lock)) < 0) {
        printf("lock failed: type:%d\n", lock.l_type);
        return -1;
    }

    switch (lock.l_type)
    {
    case F_RDLCK:
        printf("RD lock set by: %d\n", getpid());
        break;
    
    case F_WRLCK:
        printf("WR lock set by: %d\n", getpid());
        break;

    case F_UNLCK:
        printf("UNLK set by pid: %d\n", getpid());
        break;

    default:
        break;
    }

    return 0;
}