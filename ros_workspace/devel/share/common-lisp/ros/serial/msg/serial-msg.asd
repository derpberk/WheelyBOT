
(cl:in-package :asdf)

(defsystem "serial-msg"
  :depends-on (:roslisp-msg-protocol :roslisp-utils :std_msgs-msg
)
  :components ((:file "_package")
    (:file "trama" :depends-on ("_package_trama"))
    (:file "_package_trama" :depends-on ("_package"))
  ))