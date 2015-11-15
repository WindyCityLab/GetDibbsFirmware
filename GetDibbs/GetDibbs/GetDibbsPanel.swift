//
//  GetDibbsPanel.swift
//  GetDibbs
//
//  Created by Kevin McQuown on 11/8/15.
//  Copyright © 2015 Kevin McQuown. All rights reserved.
//

import Foundation
import CoreBluetooth
let serviceString = "6e400001-b5a3-f393-e0a9-e50e24dcca9e"
let sampleSizeForRSSIReading = 10
let RSSIMinimum = -55

private let sharedPanel = GetDibbsPanel()

protocol GetDibbsPanelDelegate
{
    func latestRSSIValue(value : Int);
    func fenceUpdate(isCloseToPanel : Bool);
}

class GetDibbsPanel : NSObject, CBCentralManagerDelegate
{
    var delegate : GetDibbsPanelDelegate?
    private var RSSIBuffer : RingBuffer = RingBuffer(sizeOfBuffer: sampleSizeForRSSIReading)
    private var fenceClose = false;
    
    class var sharedInstance : GetDibbsPanel
    {
        return sharedPanel;
    }
    
    var central : CBCentralManager?
    var serviceID : CBUUID = CBUUID(string: serviceString)
    
    func centralManagerDidUpdateState(central: CBCentralManager) {
        switch central.state
        {
        case CBCentralManagerState.PoweredOn:
            print("central manager on!");
            central.scanForPeripheralsWithServices([serviceID], options: [CBCentralManagerScanOptionAllowDuplicatesKey: true])
        default:
            print("something is wrong with central manager");
        }
    }
    func centralManager(central: CBCentralManager, didDiscoverPeripheral peripheral: CBPeripheral, advertisementData: [String : AnyObject], RSSI: NSNumber) {
        
        // lets smooth out the values to an average via the ring buffer
        RSSIBuffer.add(RSSI.integerValue)
//        print(RSSI.integerValue)
        if RSSIBuffer.bufferReady
        {
            delegate?.latestRSSIValue(RSSIBuffer.value())
            if (RSSIBuffer.value() > RSSIMinimum) && !fenceClose
            {
                fenceClose = true
                delegate?.fenceUpdate(fenceClose)
            }
            if (RSSIBuffer.value() < RSSIMinimum) && fenceClose
            {
                fenceClose = false
                delegate?.fenceUpdate(fenceClose)
            }
        }
    }

    func begin()
    {
        central = CBCentralManager(delegate: self, queue: nil);
    }
}
