//
//  RingBuffer.swift
//  GetDibbs
//
//  Created by Kevin McQuown on 11/8/15.
//  Copyright © 2015 Kevin McQuown. All rights reserved.
//

import Foundation

class RingBuffer
{
    private var buffer : [Int] = Array()
    private var size : Int = 0
    private var index : Int = 0
    
    var bufferReady : Bool = false
    
    init (sizeOfBuffer : Int)
    {
        size = sizeOfBuffer
        for (var i=0; i<size; i++)
        {
            buffer.append(0)
        }
    }
    
    func add(value : Int)
    {
        buffer[index] = value
        index = (index + 1) % size
        if (index == 0)  // We just wrapped around once
        {
            bufferReady = true;
        }
    }
    
    func value() -> Int
    {
        var total = 0;
        for value in buffer
        {
            total = total + value
        }
        return total / size;
    }
}