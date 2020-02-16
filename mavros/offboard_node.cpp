/**
 * @file offb_node.cpp
 * @brief offboard example node, written with mavros version 0.14.2, px4 flight
 * stack and tested in Gazebo SITL
 */

#include <ros/ros.h>
#include <geometry_msgs/PoseStamped.h>       // 几何头文件
#include <mavros_msgs/CommandBool.h>
#include <mavros_msgs/SetMode.h>
#include <mavros_msgs/State.h>

mavros_msgs::State current_state;               // 声明 当前状态变量
geometry_msgs::PoseStamped current_position;  //声明 当前位置变量

// 状态回调函数   用来保存飞控的当前状态  包括：连接状态 解锁状态 外部控制标志
void state_cb(const mavros_msgs::State::ConstPtr& msg)
{
    current_state = *msg;
}

//位置回调函数  用来保存飞控的当前位置
void position_cb(const geometry_msgs::PoseStamped::ConstPtr &current_pose)
{
  current_position= *current_pose;
}

int main(int argc, char **argv)
{
        ros::init(argc, argv, "offb_node");         // 初始化节点 offb_node
        ros::NodeHandle nh;                                  // 创建一个通讯对象 nh

        // 用来 订阅  状态state
        ros::Subscriber state_sub = nh.subscribe<mavros_msgs::State> ("mavros/state", 10, state_cb);
        //用来 订阅  当前的位置
        ros::Subscriber position_sub = nh.subscribe<geometry_msgs::PoseStamped>  ("mavros/local_position/pose",10,position_cb);
        // 用来 发布 目标位置
        ros::Publisher local_pos_pub = nh.advertise<geometry_msgs::PoseStamped> ("mavros/setpoint_position/local", 10);
        //用来 请求 飞机解锁状态
        ros::ServiceClient arming_client = nh.serviceClient<mavros_msgs::CommandBool> ("mavros/cmd/arming");
        // 用来 请求 切换FCU模式
         ros::ServiceClient set_mode_client = nh.serviceClient<mavros_msgs::SetMode> ("mavros/set_mode");

    //the setpoint publishing rate MUST be faster than 2Hz
    ros::Rate rate(20.0);                                //设置ros频率

    // 等待飞控连接
    while(ros::ok() && current_state.connected)
    {
        ros::spinOnce();                               // 一旦连接 就跳转
        rate.sleep();
    }

    //  初始化pose位置 用于发布 目标位置
    geometry_msgs::PoseStamped target_pose;
    target_pose.pose.position.x = 0;
    target_pose.pose.position.y = 0;
    target_pose.pose.position.z = 10;

    // 循环 发布 目标位置 为 target_pose
    for(int i = 100; ros::ok() && i > 0; --i)
    {
        local_pos_pub.publish(target_pose);
        ros::spinOnce();
        rate.sleep();
    }

    //  初始化offb_set_mode机载电脑模式 用于 请求 切换FCU模式
    mavros_msgs::SetMode offb_set_mode;
    offb_set_mode.request.custom_mode = "OFFBOARD";

    //  初始化  解锁=1 请求  解锁
    mavros_msgs::CommandBool arm_cmd;
    arm_cmd.request.value = true;

    ros::Time last_request = ros::Time::now();          // 获取现在的时间

    while(ros::ok())
    {
        if( current_state.mode != "OFFBOARD" && (ros::Time::now() - last_request > ros::Duration(5.0)))  // 如果不是 offboard 模式 而且 时间大于5秒
            {
            if( set_mode_client.call(offb_set_mode) && offb_set_mode.response.mode_sent)                        //请求 切换FCU模
                {
                        ROS_INFO("Offboard enabled");                                                                                                                            // 成功则打印 "Offboard enabled"
                }
            last_request = ros::Time::now();
        }
        else                                                                                                                                                                                            // 如果  已经是offboard 模式
        {
            if( !current_state.armed &&(ros::Time::now() - last_request > ros::Duration(5.0)))                          // 如果  没有解锁 而且 时间大于5秒
            {
                if( arming_client.call(arm_cmd) &&arm_cmd.response.success)                                                     // 请求 解锁
                {
                    ROS_INFO("Vehicle armed");
                }
             last_request = ros::Time::now();
            }
        }

        //判断当前位置信息，若到达（0,0,10）的位置就向x轴前进10米,即（10,0,10）
        if(current_position.pose.position.x > (-0.2) && current_position.pose.position.x < 0.2
                && current_position.pose.position.y > (-0.2) && current_position.pose.position.y < 0.2
                && current_position.pose.position.z > 9.8 && current_position.pose.position.z < 10.2 )
                 {
                        target_pose.pose.position.x = 10;
                        target_pose.pose.position.y = 0;
                        target_pose.pose.position.z =10;
                }

        //判断当前位置信息，若到达（10,0,10）的位置就到（10,10,10）
        if(current_position.pose.position.x > 9.8 && current_position.pose.position.x < 10.2
              && current_position.pose.position.y > (-0.2) && current_position.pose.position.y < 0.2
              && current_position.pose.position.z > 9.8 && current_position.pose.position.z < 10.2 )
                 {
                      target_pose.pose.position.x = 10;
                      target_pose.pose.position.y = 10;
                      target_pose.pose.position.z =10;
                 }

         //判断当前位置信息，若到达（10,10,10）的位置就到（0,10,10）
        if(current_position.pose.position.x > 9.8 && current_position.pose.position.x < 10.2
              && current_position.pose.position.y > 9.8 && current_position.pose.position.y < 10.2
              && current_position.pose.position.z > 9.8 && current_position.pose.position.z < 10.2 )
                 {
                      target_pose.pose.position.x = 0;
                      target_pose.pose.position.y = 10;
                      target_pose.pose.position.z = 10;
                 }

         //判断当前位置信息，若到达（0,10,10）的位置就到（0,0,10）
        if(current_position.pose.position.x > (-0.2) && current_position.pose.position.x < 0.2
              && current_position.pose.position.y > 9.8 && current_position.pose.position.y < 10.2
              && current_position.pose.position.z > 9.8 && current_position.pose.position.z < 10.2 )
                 {
                      target_pose.pose.position.x = 0;
                      target_pose.pose.position.y = 0;
                      target_pose.pose.position.z = 10;
                 }

                 local_pos_pub.publish(target_pose); //发布 目标位置

                 if( ros::Time::now() - last_request > ros::Duration(1.0))//时间大于1s时满足条件
                  {
                    ROS_INFO("current position \n x:%.2f\n y:%.2f\n z:%.2f",current_position.pose.position.x,
                                                                      current_position.pose.position.y,
                                                                      current_position.pose.position.z);
                    last_request = ros::Time::now(); //获取当前时间
                  }


                ros::spinOnce();
                rate.sleep();
    }

    return 0;
}
