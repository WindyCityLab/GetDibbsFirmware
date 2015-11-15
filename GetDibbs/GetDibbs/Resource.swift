//
//  Resource.swift
//  GetDibbs
//
//  Created by Kevin McQuown on 9/12/15.
//  Copyright © 2015 Kevin McQuown. All rights reserved.
//

enum ResourceType : Int {
    case ConferenceRoom = 0, PhoneBooth, BreakOutRoomm, LockOutDevice
}

class Resource : PFObject, PFSubclassing
{
    static func parseClassName() -> String {
        return "Resource";
    }
    
    @NSManaged var name : String!
    @NSManaged var uniqueID : NSNumber!
    @NSManaged var parseType : NSNumber!;
    @NSManaged var requiresCertification : NSNumber!
    
    var type : ResourceType
        {
        get {
            if let value = ResourceType(rawValue : parseType.integerValue)
            {
                return value;
            }
            else
            {
                return ResourceType.ConferenceRoom;
            }
        }
    }
}