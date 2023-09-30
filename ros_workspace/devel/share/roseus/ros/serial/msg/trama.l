;; Auto-generated. Do not edit!


(when (boundp 'serial::trama)
  (if (not (find-package "SERIAL"))
    (make-package "SERIAL"))
  (shadow 'trama (find-package "SERIAL")))
(unless (find-package "SERIAL::TRAMA")
  (make-package "SERIAL::TRAMA"))

(in-package "ROS")
;;//! \htmlinclude trama.msg.html
(if (not (find-package "STD_MSGS"))
  (ros::roseus-add-msgs "std_msgs"))


(defclass serial::trama
  :super ros::object
  :slots (_trama ))

(defmethod serial::trama
  (:init
   (&key
    ((:trama __trama) (instance std_msgs::String :init))
    )
   (send-super :init)
   (setq _trama __trama)
   self)
  (:trama
   (&rest __trama)
   (if (keywordp (car __trama))
       (send* _trama __trama)
     (progn
       (if __trama (setq _trama (car __trama)))
       _trama)))
  (:serialization-length
   ()
   (+
    ;; std_msgs/String _trama
    (send _trama :serialization-length)
    ))
  (:serialize
   (&optional strm)
   (let ((s (if strm strm
              (make-string-output-stream (send self :serialization-length)))))
     ;; std_msgs/String _trama
       (send _trama :serialize s)
     ;;
     (if (null strm) (get-output-stream-string s))))
  (:deserialize
   (buf &optional (ptr- 0))
   ;; std_msgs/String _trama
     (send _trama :deserialize buf ptr-) (incf ptr- (send _trama :serialization-length))
   ;;
   self)
  )

(setf (get serial::trama :md5sum-) "91d4cca73e4fe47672e54da2c75435ef")
(setf (get serial::trama :datatype-) "serial/trama")
(setf (get serial::trama :definition-)
      "std_msgs/String trama

================================================================================
MSG: std_msgs/String
string data

")



(provide :serial/trama "91d4cca73e4fe47672e54da2c75435ef")


