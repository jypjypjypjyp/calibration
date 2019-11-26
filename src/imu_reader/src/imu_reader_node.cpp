#include <ros/ros.h>
#include <sensor_msgs/Imu.h>

#include <fstream>
#include <string>
#include <thread>
#include <iomanip>

std::string IMU_TOPIC;
std::string IMU_NAME;
std::string DATA_SAVE_PATH;
std::ofstream file;
bool end = false;
void imu_callback(const sensor_msgs::ImuConstPtr &msg)
{
    std::cout<<"OK!"<<std::endl;
    file << std::setprecision(10) << msg->header.stamp.toSec() << ',';
    file << msg->header.stamp.toNSec() << ',';
    file << msg->angular_velocity.x << ',';
    file << msg->angular_velocity.y << ',';
    file << msg->angular_velocity.z << ',';
    file << msg->linear_acceleration.x << ',';
    file << msg->linear_acceleration.y << ',';
    file << msg->linear_acceleration.z << '\n';
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
    ros::init(argc, argv, "imu_reader_node");
    ros::NodeHandle n("~");

    n.getParam("imu_topic", IMU_TOPIC);
    n.getParam("imu_name", IMU_NAME);
    n.getParam("data_save_path", DATA_SAVE_PATH);

    file.open(DATA_SAVE_PATH+IMU_NAME+".csv");

    ros::Subscriber ros_tutorial_sub = n.subscribe(IMU_TOPIC, 100, imu_callback);

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
