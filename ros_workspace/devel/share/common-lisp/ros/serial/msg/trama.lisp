; Auto-generated. Do not edit!


(cl:in-package serial-msg)


;//! \htmlinclude trama.msg.html

(cl:defclass <trama> (roslisp-msg-protocol:ros-message)
  ((trama
    :reader trama
    :initarg :trama
    :type std_msgs-msg:String
    :initform (cl:make-instance 'std_msgs-msg:String)))
)

(cl:defclass trama (<trama>)
  ())

(cl:defmethod cl:initialize-instance :after ((m <trama>) cl:&rest args)
  (cl:declare (cl:ignorable args))
  (cl:unless (cl:typep m 'trama)
    (roslisp-msg-protocol:msg-deprecation-warning "using old message class name serial-msg:<trama> is deprecated: use serial-msg:trama instead.")))

(cl:ensure-generic-function 'trama-val :lambda-list '(m))
(cl:defmethod trama-val ((m <trama>))
  (roslisp-msg-protocol:msg-deprecation-warning "Using old-style slot reader serial-msg:trama-val is deprecated.  Use serial-msg:trama instead.")
  (trama m))
(cl:defmethod roslisp-msg-protocol:serialize ((msg <trama>) ostream)
  "Serializes a message object of type '<trama>"
  (roslisp-msg-protocol:serialize (cl:slot-value msg 'trama) ostream)
)
(cl:defmethod roslisp-msg-protocol:deserialize ((msg <trama>) istream)
  "Deserializes a message object of type '<trama>"
  (roslisp-msg-protocol:deserialize (cl:slot-value msg 'trama) istream)
  msg
)
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql '<trama>)))
  "Returns string type for a message object of type '<trama>"
  "serial/trama")
(cl:defmethod roslisp-msg-protocol:ros-datatype ((msg (cl:eql 'trama)))
  "Returns string type for a message object of type 'trama"
  "serial/trama")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql '<trama>)))
  "Returns md5sum for a message object of type '<trama>"
  "91d4cca73e4fe47672e54da2c75435ef")
(cl:defmethod roslisp-msg-protocol:md5sum ((type (cl:eql 'trama)))
  "Returns md5sum for a message object of type 'trama"
  "91d4cca73e4fe47672e54da2c75435ef")
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql '<trama>)))
  "Returns full string definition for message of type '<trama>"
  (cl:format cl:nil "std_msgs/String trama~%~%================================================================================~%MSG: std_msgs/String~%string data~%~%~%"))
(cl:defmethod roslisp-msg-protocol:message-definition ((type (cl:eql 'trama)))
  "Returns full string definition for message of type 'trama"
  (cl:format cl:nil "std_msgs/String trama~%~%================================================================================~%MSG: std_msgs/String~%string data~%~%~%"))
(cl:defmethod roslisp-msg-protocol:serialization-length ((msg <trama>))
  (cl:+ 0
     (roslisp-msg-protocol:serialization-length (cl:slot-value msg 'trama))
))
(cl:defmethod roslisp-msg-protocol:ros-message-to-list ((msg <trama>))
  "Converts a ROS message object to a list"
  (cl:list 'trama
    (cl:cons ':trama (trama msg))
))
