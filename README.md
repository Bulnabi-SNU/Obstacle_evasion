<h1 align = "center"> PX4-Apriltag based auto landing control </h1>

## Demonstration & Processing pipeline description

<a href="https://youtube.com/shorts/I7M_TE6y5zg?feature=share"><p align="center">
    
</p></a>

## Getting Started

### Step 1 - Prerequisites
Jetson Xavier NX (Jetpack 5.0.2) - with PX4 board connected with GPIO pins   
-look for [HOW TO CONNECT JETSON-PX4] under  
ROS2 Foxy   
numpy  
FastDDS(MicroXRCEDDS)  
[PX4-Autopilot](https://github.com/PX4/PX4-Autopilot)


### Step 2 - Configure Hardware

#### HOW TO ENABLE JETSON GPIO & CAMERA PIN FOR IMX477 CAMERA BOARD
configure mission hardware pin connections(PWM inputs via GPIO -- Ground must be shared)
connect ZED 2i camera via USB

### Step 3 - Install auto_landing package
  ```
    cd ~/ros2_ws/src/
    git clone https://github.com/One-star11/Obstacle_evasion.git
    cd ..
    colcon build --symlink-install
    source ~/ros2_ws/install/setup.bash
  ```


### Step 4 - Start nodes
```
    #all in seperate terminals
```


