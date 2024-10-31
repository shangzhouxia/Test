#include <ctime>
#include <fstream>
#include <google/protobuf/util/time_util.h>
#include <iostream>
#include <string>
#include "stdio.h"

#include "nest.pb.h"

using namespace std;

using google::protobuf::util::TimeUtil;





int main()
{
    nest::TaskList taskList;
    nest::Task task;
    nest::TaskStatus status;

    status = nest::TaskStatus::IN_PROGRESS;

    // 初始化数据
    task.set_id("No.1");
    task.set_title("test nest");
    task.set_description("test nest by zhuguanglei");
    task.set_status(status);

    *task.mutable_due_date() = TimeUtil::SecondsToTimestamp(time(NULL));

    taskList.add_tasks()->CopyFrom(task);


    cout<< "data size: " << taskList.tasks_size() << endl;

    for (int index = 0; index < taskList.tasks_size(); index ++) {
        cout << taskList.tasks(index).id() << endl;
        cout << taskList.tasks(index).title() << endl;
        cout << taskList.tasks(index).description() << endl;
        cout << taskList.tasks(index).status() << endl;
        cout << "update: " << TimeUtil::ToString(taskList.tasks(index).due_date())<<endl;
    }

    // 写入数据到文件，方便后续其他模块解析
    std::ofstream output("task_list_data.bin", std::ios::out | std::ios::binary);
    taskList.SerializeToOstream(&output);
    output.close();


    // 释放资源
    google::protobuf::ShutdownProtobufLibrary();

    return 0;
}