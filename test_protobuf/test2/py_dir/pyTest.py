import google.protobuf
import nest_pb2  # 导入生成的 protobuf 代码

def load_task_list_from_file(filename):
    # 从文件中读取 TaskList 并反序列化
    task_list = nest_pb2.TaskList()
    with open(filename, 'rb') as f:
        task_list.ParseFromString(f.read())
    return task_list

def main():
    # 从文件加载 TaskList
    loaded_task_list = load_task_list_from_file('../task_list_data.bin')
    print("Loaded Task List:")
    
    for task in loaded_task_list.tasks:
        print("ID:", task.id)
        print("Title:", task.title)
        print("Description:", task.description)
        print("Status:", task.status)

if __name__ == "__main__":
    main()
