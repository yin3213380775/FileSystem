#include "FS.h"
void FileManagement::add_index(int i_number) {
    /*
     *调用磁盘新写的函数即可
     */


}

void FileManagement::delete_index() {
    /*
     * 删除索引项，可以使用二分查找并且删除
     */
}

void FileManagement::print_current_dir() {
    cout<<current_dir;
}

bool FileManagement::rename_file(int i_number, string new_name) {
    return disk->modify_file_name(i_number,new_name);
}

bool FileManagement::create(bool file_type,string file_name) {
    /*
     * 新建文件夹、文件其实都是相当于让磁盘分配空间，因此直接调用磁盘的函数即可
     * true代表普通文件,false代表目录文件
     */
    if(file_type){
        disk->allocateBlock_File(file_name);
    } else{
        disk->allocateBlock_Dir(file_name);
    }
}

bool FileManagement::remove(string file_name, int i_number) {
    /*
     * 回收磁盘空间，删除对应inode节点
     */
}
bool FileManagement::recycle_file(string file_name)  {
        disk->modify_file_recycled();
}

string FileManagement::get_dir_content(int i_number) {
    disk->read_file(i_number);
    for(auto i:dir_input_buffer){
        dir_content.append(i.file_name);
        dir_content.push_back('\t');
        dir_content.append(i.modified_time);
        dir_content.push_back('\t');
        //dir_content.append();此处需要增加文件类型
        dir_content.push_back('\n');
    }
    return dir_content;
}

string FileManagement::get_file_content() {
    for (auto i:input_buffer) {
        file_content.push_back(i);
    }
    return file_content;
}

string FileManagement::cd_dir(string file_name) {
    //bool exist= false;
    /*
     * 根据文件夹名称进入相应的目录
     * 返回的就是进入目录的相关信息，用string存储，可以在GUI端显示
     */
    for(auto i:dir_input_buffer){
        if(i.file_name==file_name){
            /*
             * 在这里要修改一下当前目录和上级目录
             * 在这里要修改一下当前目录和上级目录的inode号
             */
            parent_inode=ope_inode;
            ope_inode=i.i_number;
            //exist= true;
            break;
        }
    }
    disk->read_file(ope_inode);
    return get_dir_content(ope_inode);//返回子目录的内容
}

// 按照文件名进行比较
bool compareByName(const Inode& a, const Inode& b) {
    return a.file_name[0] < b.file_name[0];
}
// 按照文件大小进行比较
bool compareBySize(const Inode& a, const Inode& b) {
    return a.file_size < b.file_size;
}
// 按照修改日期进行比较
bool compareByModifiedTime(const Inode& a, const Inode& b) {
    return a.modified_time.compare(b.modified_time) < 0;
}

void FileManagement::sort_index(int choice) {
    /*
     * 删除索引项，可以使用二分查找并且删除
     * 使用STL算法，按照关键字进行快速排序
     */
    switch (choice) {
        case 1:
            sort(disk->dir_output_buffer, disk->dir_output_buffer + 20, compareByName);
            break;
        case 2:
            sort(disk->dir_output_buffer, disk->dir_output_buffer + 20, compareBySize);
            break;
        case 3:
            sort(disk->dir_output_buffer, disk->dir_output_buffer + 20, compareByModifiedTime);
            break;
    }
}

FileManagement::FileManagement(Inode* root) : ope_inode(root->i_number), parent_inode(0), disk(nullptr), dir_content(""), file_content(""), current_dir("") {
    // 在构造函数中可以进行其他初始化工作
    // 例如，对输入缓冲区和目录项输入缓冲区进行初始化
    // 你可能还需要初始化其他成员，具体取决于你的设计需求
    for (int i = 0; i < 20; ++i) {
        dir_input_buffer[i].index = 0;
        // 对其他成员的初始化
    }
}

// 为了示例目的，这里没有对 disk 进行初始化
// 你可能需要根据实际情况在构造函数中进行 disk 的初始化

