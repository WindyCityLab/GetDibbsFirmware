//
//  Group.swift
//  GetDibbs
//
//  Created by Kevin McQuown on 9/12/15.
//  Copyright © 2015 Kevin McQuown. All rights reserved.
//

class Group : PFObject, PFSubclassing
{
    static func parseClassName() -> String {
        return "Group";
    }
}