import rclpy
from rclpy.node import Node

from std_msgs.msg import Int16 

class MinimalPublisher(Node):

	def __init__(self):
		super().__init__('minimal_publisher')
		self.publisher_= self.create_publisher(Int16, 'servo_topic', 10)
		timer_period = 1
		self.timer = self.create_timer(timer_period, self.timer_callback)
		self.i = 0

	def timer_callback(self):
		msg = Int16()
		mode = int(input("mode:"))
		msg.data = mode
		self.publisher_.publish(msg)
		self.get_logger().info('Publishing: "%d"' % msg.data)

def main(args=None):
	rclpy.init(args=args)

	minimal_publisher = MinimalPublisher()

	rclpy.spin(minimal_publisher)

	minimal_publisher.destroy_node()
	rclpy.shutdown()

if __name__ == '__main__':
	main()
