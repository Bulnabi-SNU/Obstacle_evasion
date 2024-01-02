import rclpy
from rclpy.node import Node

from std_msgs.msg import Int16

import Jetson.GPIO as GPIO
from time import sleep

servoPin=33
SERVO_MAX_DUTY=13
SERVO_MIN_DUTY=3

Mission_Time=10
t_timer=0.5
Angle_min=0
Angle_max=180

GPIO.setmode(GPIO.BOARD)
GPIO.setup(servoPin, GPIO.OUT)
GPIO.setwarnings(False)
servo = GPIO.PWM(servoPin, 50)
servo.start(Angle_min)

def setServoPos(degree):

	if degree > Angle_max:
		degree = Angle_max
	elif degree < Angle_min:
		degree = Angle_min

	duty = SERVO_MIN_DUTY+(degree*(SERVO_MAX_DUTY-SERVO_MIN_DUTY)/180.0)

	print("Degree: {} to {}(Duty)".format(degree, duty))

	servo.ChangeDutyCycle(duty)

class Mission(Node): 

	def __init__(self):
		super().__init__('mission_equip')
		self.subscription = self.create_subscription(
			Int16,
			'servo_topic',
			self.mission_callback,
			10)
		self.subscription		
		self.angle=Angle_min
		self.mode=0
		self.create_timer(t_timer,self.timer_callback)
		

#msg.data 
#0000:set zero position : mode 0
#1111:extend equipment  : mode 1
#22222:reduce equipment : mode 2
#0~180: desire angle
	
	def mission_callback(self,msg):
		
		if (msg.data==0000):
			self.mode=0

		elif (msg.data==1111):
            self.mode=1
		
		elif (msg.data==2222):
			self.mode=2

		elif (msg.data>=0 and msg.data<=180):
			self.angle=msg.data
			setServoPos(self.angle)


    def timer_callback(self):
        if(self.mode==0):
            setServoPos(0)
	    
        elif(self.mode==1):
            self.angle=self.angle+180/Mission_Time*t_timer
	    
	    elif(self.mode==2):
            self.angle=self.angle-180/Mission_Time*t_timer
	    
	
def main(args=None): 
	rclpy.init(args=args)
	mission = Mission()
	
	rclpy.spin(mission)


	mission.destroy_node()
	rclpy.shutdown()
	servo.stop()
	GPIO.cleanup()

if __name__ == '__main__':
	main()
