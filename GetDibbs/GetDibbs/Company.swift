//
//  Company.swift
//  GetDibbs
//
//  Created by Kevin McQuown on 9/12/15.
//  Copyright © 2015 Kevin McQuown. All rights reserved.
//

class Company : PFObject, PFSubclassing
{
    static func parseClassName() -> String {
        return "Company";
    }
    
    @NSManaged var name : String!;
    @NSManaged var red : NSNumber!;
    @NSManaged var green : NSNumber!;
    @NSManaged var blue : NSNumber!;
    @NSManaged var uniqueID : NSNumber!;
    
    var id : NSData!
    {
        get {
            var value = uniqueID.integerValue;
            return NSData(bytes: &value , length: 2);
        }
    }
}
