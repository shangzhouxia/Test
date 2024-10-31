#include "stdio.h"
#include "proj.pb.h"
#include <fstream>
#include <iostream>
#include <string>

using namespace std;

int main(int argc, char* argv[])
{
/*     SolingDv_Msg dvMsg;
    Loading_Msg loadMsg;
    IPErrMsg ipMsg;

    SolingDv_Msg readMsg;

    if (argc != 2) {
      cerr << "Usage:  " << argv[0] << " data file" << endl;
      return -1;
    }

    loadMsg.set_bpu_load(66.6);
    loadMsg.set_cpu_load(77.7);
    loadMsg.set_gpu_load(88.8);
    loadMsg.set_mem_load(99.9);

    ipMsg.set_cf_err(true);
    ipMsg.set_ncf_err(false);

    dvMsg.set_allocated_loading_msg(&loadMsg);
    dvMsg.set_allocated_ip_err_msg(&ipMsg); */

/*     fstream output(argv[1], ios::out | ios::trunc | ios::binary);
    dvMsg.SerializePartialToOstream(&output); */

/*     fstream input(argv[1], ios::in | ios::binary);
    readMsg.ParseFromIstream(&input);

    cout<<"BPU Load: "<<readMsg.loading_msg().bpu_load()<<endl;
    cout<<"CPU Load: "<<readMsg.loading_msg().cpu_load()<<endl;
    cout<<"GPU Load: "<<readMsg.loading_msg().gpu_load()<<endl;
    cout<<"MEM Load: "<<readMsg.loading_msg().mem_load()<<endl;

    cout<<"CF Err: "<<readMsg.ip_err_msg().cf_err()<<endl;
    cout<<"NCF Err: "<<readMsg.ip_err_msg().ncf_err()<<endl; */

    Loading_Msg loadMsg;
    Loading_Msg readMsg;
    string line;

    loadMsg.set_bpu_load(66.6);
    loadMsg.set_cpu_load(77.7);
    loadMsg.set_gpu_load(88.8);
    loadMsg.set_mem_load(99.9);

    //fstream output(argv[1], ios::out | ios::trunc | ios::binary);
    loadMsg.SerializePartialToString(&line);

/*     cout<<"before: "<<endl;
    cout<<"BPU Load: "<<readMsg.bpu_load()<<endl;
    cout<<"CPU Load: "<<readMsg.cpu_load()<<endl;
    cout<<"GPU Load: "<<readMsg.gpu_load()<<endl;
    cout<<"MEM Load: "<<readMsg.mem_load()<<endl; */

    //fstream input(argv[1], ios::in | ios::binary);
    if (!readMsg.ParseFromString(line)) {
      cerr << "Failed to parse address book." << endl;
      return -1;
    }

    cout<<"after: "<<endl;
    cout<<"BPU Load: "<<readMsg.bpu_load()<<endl;
    cout<<"CPU Load: "<<readMsg.cpu_load()<<endl;
    cout<<"GPU Load: "<<readMsg.gpu_load()<<endl;
    cout<<"MEM Load: "<<readMsg.mem_load()<<endl;
    

    google::protobuf::ShutdownProtobufLibrary();

    return 0;
}
