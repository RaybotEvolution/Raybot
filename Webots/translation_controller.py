"""keyboard_teleop controller."""

# You may need to import some classes of the controller module. Ex:
#  from controller import Robot, Motor, DistanceSensor
from controller import Robot,Keyboard, Supervisor

# create the Robot and supervisor instance.
robot = Robot()
_supervisor = Supervisor()

robot_node = _supervisor.getFromDef('humanoid')
transl_feild = robot_node.getField('translation')
rotation_feild = robot_node.getField('rotation') 
# get the time step of the current world.
TIME_STEP = 32

kb=Keyboard()
kb.enable(TIME_STEP)

# Left arm motor list
left_arm_motors = []
right_arm_motors = []
left_arm_motor_pos = []
right_arm_motor_pos = []

left_arm_names= ['il1','il2','il3','il4','il5','il6','il7','il8',]
right_arm_names = ['ir1','ir2','ir3','ir4','ir5','ir6','ir7','ir8',]

torso_motor_name = 'ia2'
torso_motor = robot.getDevice(torso_motor_name)
# Set initial velocities and positions to zero
for i in range(8):
    left_arm_motors.append(robot.getDevice(left_arm_names[i]))
    right_arm_motors.append(robot.getDevice(right_arm_names[i]))




# flags 
key_enable = True
move_robot = False
way_point = None
#coounter 
i = 0
new_transl = [0, 0, 0]


while robot.step(TIME_STEP) != -1:
    key=kb.getKey()
    # print(key)
    
    if (key == 65) and key_enable:
        print("Movement request sent")
        key_enable = False
        move_robot = True
        way_point = "A"
        present_transl = robot_node.getPosition()
        i = 0
    elif (key== 66) and key_enable:
        print("Movement request sent")
        key_enable = False
        move_robot = True
        way_point = "B"
        present_transl = robot_node.getPosition()
        i = 0
    elif (key== 67) and key_enable:
        print("Movement request sent")
        key_enable = False
        move_robot = True
        way_point = "C"
        present_transl = robot_node.getPosition()
        i = 0
    elif (key== 68) and key_enable:
        print("Movement request sent")
        key_enable = False
        move_robot = True
        way_point = "D"
        present_orient = [0,0,0,1]
        i = 0
    elif (key== 69) and key_enable:
        print("Movement request sent")
        key_enable = False
        move_robot = True
        way_point = "E"
        present_orient = [0,0,-0.69,0.69]
        i = 0
    elif (key==Keyboard.UP) and key_enable:
        # torso motor device
        
        print("Moving to first pose")
        # #move torso motor to 20 degrees
        torso_motor.setPosition(-0.3)
        #set positon one for left_arm 
        left_arm_motors[0].setPosition(-1.36)
        left_arm_motors[1].setPosition(-0.27)
        left_arm_motors[2].setPosition(-1.80)
        left_arm_motors[3].setPosition(-0.30)
        left_arm_motors[4].setPosition(-0.17)
        left_arm_motors[5].setPosition(1.25)
        #Set postion one for right_arm 
        right_arm_motors[0].setPosition(1.36)
        right_arm_motors[1].setPosition(0.27)
        right_arm_motors[2].setPosition(1.80)
        right_arm_motors[3].setPosition(0.30)
        right_arm_motors[4].setPosition(0.17)
        right_arm_motors[5].setPosition(-1.25)
        print("Movement finish")
    elif (key==Keyboard.DOWN) and key_enable:
        print("Moving to second pose")
        torso_motor.setPosition(-0.95)
        #set positon one for left_arm 
        left_arm_motors[0].setPosition(-1.36)
        left_arm_motors[1].setPosition(-0.27)
        left_arm_motors[2].setPosition(-1.80)
        left_arm_motors[3].setPosition(-0.30)
        left_arm_motors[4].setPosition(-0.17)
        left_arm_motors[5].setPosition(0.55)
        #Set postion one for right_arm 
        right_arm_motors[0].setPosition(1.36)
        right_arm_motors[1].setPosition(0.27)
        right_arm_motors[2].setPosition(1.80)
        right_arm_motors[3].setPosition(0.30)
        right_arm_motors[4].setPosition(0.17)
        right_arm_motors[5].setPosition(-0.55)
        print("Movement finish")
    elif (key==Keyboard.LEFT) and key_enable:
        print("Moving to second pose")
        torso_motor.setPosition(0)
        #set positon one for left_arm 
        left_arm_motors[0].setPosition(0)
        left_arm_motors[1].setPosition(0)
        left_arm_motors[2].setPosition(0)
        left_arm_motors[3].setPosition(0)
        left_arm_motors[4].setPosition(0)
        left_arm_motors[5].setPosition(0)
        #Set postion one for right_arm 
        right_arm_motors[0].setPosition(0)
        right_arm_motors[1].setPosition(0)
        right_arm_motors[2].setPosition(0)
        right_arm_motors[3].setPosition(0)
        right_arm_motors[4].setPosition(0)
        right_arm_motors[5].setPosition(0)
        print("Movement finish")
    else :
        pass

    
        
    if move_robot:
        if way_point == "A" and i<30:
            print("Moving robot to point A")
            new_transl = present_transl
            new_transl[0] -=0.01
            transl_feild.setSFVec3f(new_transl)
        elif way_point == "B" and i<345:
            print("Moving robot to point B")
            new_transl = present_transl
            new_transl[1] -=0.01
            transl_feild.setSFVec3f(new_transl)
        elif way_point == "C" and i<30:
            print("Moving robot to point C")
            new_transl = present_transl
            new_transl[0] +=0.01
            transl_feild.setSFVec3f(new_transl)
        elif way_point == "D" and i<60:
            print("Moving robot to point D")
            present_orient[2] -=0.01
            present_orient[3] +=0.01
            rotation_feild.setSFRotation(present_orient)
        elif way_point == "E" and i<64:
            print("Moving robot to point E",i)
            present_orient[2] +=0.01
            present_orient[3] -=0.01
            rotation_feild.setSFRotation(present_orient)
        else:
            key_enable = True
        i += 1
                
# Enter here exit cleanup code.
