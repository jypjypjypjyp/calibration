#include <ros/ros.h>
#include <message_filters/subscriber.h> 
#include <message_filters/time_synchronizer.h>
#include <sensor_msgs/Imu.h>

#include <fstream>
#include <string>
#include <thread>
#include <iomanip>

std::string IMU_TOPIC1;
std::string IMU_NAME1;
std::string IMU_TOPIC2;
std::string IMU_NAME2;
std::string DATA_SAVE_PATH;
std::ofstream file;
bool end = false;
void imu_callback(const sensor_msgs::ImuConstPtr &msg1,const sensor_msgs::ImuConstPtr &msg2)
{
    std::cout<<"OK!"<<std::endl;
    file << std::setprecision(18) << msg1->header.stamp.toSec() << ',';
    file << msg1->header.stamp.toNSec() << ',';
    file << msg1->angular_velocity.x << ',';
    file << msg1->angular_velocity.y << ',';
    file << msg1->angular_velocity.z << ',';
    file << msg1->linear_acceleration.x << ',';
    file << msg1->linear_acceleration.y << ',';
    file << msg1->linear_acceleration.z << '，';
    file << msg2->angular_velocity.x << ',';
    file << msg2->angular_velocity.y << ',';
    file << msg2->angular_velocity.z << ',';
    file << msg2->linear_acceleration.x << ',';
    file << msg2->linear_acceleration.y << ',';
    file << msg2->linear_acceleration.z << '\n';
}

void key_listener()
{
    char key;
    while (1)
    {
        std::cin >> key;
        if (key == 'q')
        {
            end = true;
            break;
        }
    }
}

int main(int argc, char **argv)
{
    ros::init(argc, argv, "imu_reader_sync_node");
    ros::NodeHandle n("~");

    n.getParam("imu_topic1", IMU_TOPIC1);
    n.getParam("imu_name1", IMU_NAME1);
    n.getParam("imu_topic2", IMU_TOPIC2);
    n.getParam("imu_name2", IMU_NAME2);
    n.getParam("data_save_path", DATA_SAVE_PATH);

    file.open(DATA_SAVE_PATH+"sync.csv");
    std::cout<<IMU_NAME1<<std::endl;
    std::cout<<IMU_NAME2<<std::endl;

    message_filters::Subscriber<sensor_msgs::Imu> imu1(n, IMU_TOPIC1, 10);
    message_filters::Subscriber<sensor_msgs::Imu> imu2(n, IMU_TOPIC2, 10);
    message_filters::TimeSynchronizer<sensor_msgs::Imu,sensor_msgs::Imu> sync(imu1, imu2, 100);
    sync.registerCallback(boost::bind(&imu_callback, _1, _2));

    std::thread keyControl_thread(key_listener);

    ros::Rate loop(200 * 250);
    while (!end)
    {
        loop.sleep();
        ros::spinOnce();
    }
    file.close();
    return 0;
}
