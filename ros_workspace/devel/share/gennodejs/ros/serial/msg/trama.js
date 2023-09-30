// Auto-generated. Do not edit!

// (in-package serial.msg)


"use strict";

const _serializer = _ros_msg_utils.Serialize;
const _arraySerializer = _serializer.Array;
const _deserializer = _ros_msg_utils.Deserialize;
const _arrayDeserializer = _deserializer.Array;
const _finder = _ros_msg_utils.Find;
const _getByteLength = _ros_msg_utils.getByteLength;
let std_msgs = _finder('std_msgs');

//-----------------------------------------------------------

class trama {
  constructor(initObj={}) {
    if (initObj === null) {
      // initObj === null is a special case for deserialization where we don't initialize fields
      this.trama = null;
    }
    else {
      if (initObj.hasOwnProperty('trama')) {
        this.trama = initObj.trama
      }
      else {
        this.trama = new std_msgs.msg.String();
      }
    }
  }

  static serialize(obj, buffer, bufferOffset) {
    // Serializes a message object of type trama
    // Serialize message field [trama]
    bufferOffset = std_msgs.msg.String.serialize(obj.trama, buffer, bufferOffset);
    return bufferOffset;
  }

  static deserialize(buffer, bufferOffset=[0]) {
    //deserializes a message object of type trama
    let len;
    let data = new trama(null);
    // Deserialize message field [trama]
    data.trama = std_msgs.msg.String.deserialize(buffer, bufferOffset);
    return data;
  }

  static getMessageSize(object) {
    let length = 0;
    length += std_msgs.msg.String.getMessageSize(object.trama);
    return length;
  }

  static datatype() {
    // Returns string type for a message object
    return 'serial/trama';
  }

  static md5sum() {
    //Returns md5sum for a message object
    return '91d4cca73e4fe47672e54da2c75435ef';
  }

  static messageDefinition() {
    // Returns full string definition for message
    return `
    std_msgs/String trama
    
    ================================================================================
    MSG: std_msgs/String
    string data
    
    `;
  }

  static Resolve(msg) {
    // deep-construct a valid message object instance of whatever was passed in
    if (typeof msg !== 'object' || msg === null) {
      msg = {};
    }
    const resolved = new trama(null);
    if (msg.trama !== undefined) {
      resolved.trama = std_msgs.msg.String.Resolve(msg.trama)
    }
    else {
      resolved.trama = new std_msgs.msg.String()
    }

    return resolved;
    }
};

module.exports = trama;
