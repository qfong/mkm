#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/unistd.h>
#include <asm/syscall.h>
#include <linux/syscalls.h>
#include <linux/proc_fs.h>
#include <linux/mount.h>

/*
* 返回第一个关联的可执行文件的inode号。
*/
unsigned long get_inode(void)
{
    struct dentry *dentry = NULL;
    struct vfsmount *mnt = NULL;
    struct vm_area_struct *vma;
    down_read(&current->mm->mmap_sem);


    vma = current->mm->mmap;

    while(vma){
        if((vma->vm_flags & VM_EXECUTABLE) && vma->vm_file){//得到代码段
            break;
        }
        vma = vma->vm_next;
    }
    if(vma){
        mnt = mntget(vma->vm_file->f_path.mnt);
        dentry = dget(vma->vm_file->f_path.dentry);//代码段映射文件的dentry
    }

    up_read(&current->mm->mmap_sem);

    if(dentry){
        return dentry->d_inode->i_ino;
    }
    return 0;
}
