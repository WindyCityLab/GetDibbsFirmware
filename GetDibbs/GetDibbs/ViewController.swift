//
//  ViewController.swift
//  GetDibbs
//
//  Created by Kevin McQuown on 11/8/15.
//  Copyright © 2015 Kevin McQuown. All rights reserved.
//

import UIKit

class ViewController: UIViewController {

    var conferencePanel : SparkDevice?
    
    override func viewDidLoad() {
        super.viewDidLoad()
        SparkCloud.sharedInstance().loginWithUser("kevin@thewcl.com", password: "pearson#dewitt1") { (error) -> Void in
            if let e = error
            {
                print("login failed: \(e.localizedDescription)");
            }
            else
            {
                SparkCloud.sharedInstance().getDevice("350038001547343339383037", completion: { (device, error) -> Void in
                    if (error == nil)
                    {
                        self.conferencePanel = device!
                        print("attached to device!")
                    }
                    else
                    {
                        print("Error: \(error.localizedDescription)")
                    }
                })
            }
        }
    }

    @IBAction func directionButtonTapped(sender: UIButton) {
        switch sender.tag
        {
        case 0:
            let args = ["up"]
            conferencePanel?.callFunction("setCursor", withArguments: args, completion: { (number, error) -> Void in
            })
        case 1:
            let args = ["right"]
            conferencePanel?.callFunction("setCursor", withArguments: args, completion: { (number, error) -> Void in
            })
        case 2:
            let args = ["down"]
            conferencePanel?.callFunction("setCursor", withArguments: args, completion: { (number, error) -> Void in
            })
        case 3:
            let args = ["left"]
            conferencePanel?.callFunction("setCursor", withArguments: args, completion: { (number, error) -> Void in
            })
        default:
            ()
        }
    }
    override func didReceiveMemoryWarning() {
        super.didReceiveMemoryWarning()
        // Dispose of any resources that can be recreated.
    }


}

