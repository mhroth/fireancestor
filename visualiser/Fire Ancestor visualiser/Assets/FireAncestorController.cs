﻿using UnityEngine;
using System.Collections;
using System.Collections.Generic;
using System;
using SharpOSC;

// Thread-safe message queue
public class SendMessageQueue {
	private readonly object _msgQueueSync = new object();
	private readonly Queue<SharpOSC.OscMessage> _msgQueue = new Queue<SharpOSC.OscMessage>();

	public SharpOSC.OscMessage GetNextMessage() {
		lock (_msgQueueSync) {
			return (_msgQueue.Count != 0) ? _msgQueue.Dequeue() : null;
		}
	}

	public void AddMessage(OscMessage msg) {
		lock (_msgQueueSync) {
			_msgQueue.Enqueue(msg);
		}
	}
}

public class FireAncestorController : MonoBehaviour {

	public GameObject layerPrefab;
	public int layersCount = 25;
	public float ySpacing = 7.2f;
	public float xOffset = -19.608333333f;
	public float zOffset = -11.608333333f;
	private List<GameObject> layers = new List<GameObject>();
	private List<LayerController> layerControllers = new List<LayerController>();
	private ParticleSystem poofer;
	private bool isOSCListning = false;
	private List<OscMessage> messageQueue = new List<OscMessage> ();
	public GameObject faParent;
	public float emissionRate = 100;
	public float rotationSpeed = 0.0f;

	public readonly SendMessageQueue msgQueue = new SendMessageQueue();

	public class FloatMessage {
		public string receiverName;
		public float value;

		public FloatMessage(string name, float x) {
			receiverName = name;
			value = x;
		}
	}
	public delegate void FloatMessageReceived(FloatMessage message);
	public FloatMessageReceived FloatReceivedCallback;

	// Use this for initialization
	void Start () {

		createFireAncestor ();

		OSCListen ();

		Application.runInBackground = true;
	}

	void OSCListen() {
		
		isOSCListning = true;

		HandleOscPacket callback = delegate(OscPacket packet) {

			var message = (OscMessage)packet;

			msgQueue.AddMessage(message);

		};

		var listener = new UDPListener(55555, callback);
	}


	void poofPoofer (int index, int direction, float value) {
		
		LayerController layerController = layerControllers[index];

		poofer = layerController.pooferCW;
		if (direction == 1) {
			poofer = layerController.pooferCCW;
		}

		int emit = Mathf.RoundToInt(value * emissionRate);

		poofer.Emit (emit);

		layerController.poofPoofer ();


	}

	void handleOSCMessage(OscMessage message) {
		string type = message.Address;

//		Debug.Log("Received poofer "+message+" "+type);

		if (type.StartsWith("/poofer_")) {

			int index;
			float value;

			string suffix = type.Split('_')[1];
			//messsy but let's be sure here....
			//alternative would be Int32.TryParse but I'm scared that'll accept things that it shouldn't
			if (suffix == "0" ||
				suffix == "1" ||
				suffix == "2" ||
				suffix == "3" ||
				suffix == "4" ||
				suffix == "5" ||
				suffix == "6" ||
				suffix == "7" ||
				suffix == "8" ||
				suffix == "9" ||
				suffix == "10" ||
				suffix == "11" ||
				suffix == "12" ||
				suffix == "13" ||
				suffix == "14" ||
				suffix == "15" ||
				suffix == "16" ||
				suffix == "17" ||
				suffix == "18" ||
				suffix == "19" ||
				suffix == "20" ||
				suffix == "21" ||
				suffix == "22" ||
				suffix == "23" ||
				suffix == "24" ||
				suffix == "25") {

				value = 0.0f;

				if (message.Arguments[0].GetType() == typeof(System.Single)) {
					value = (float)message.Arguments[0];
				} else if (message.Arguments[0].GetType() == typeof(System.Int32)) {
					value = (float)((int)message.Arguments[0]);
				}

				index = Int32.Parse(suffix);

//				string direction = type.Split ('_') [2];
//				int directionInt = 0;
//
//				if (direction == "0" || direction == "1") {
//					directionInt = Int32.Parse (direction);
//				} else {
//					Debug.Log ("Bad or no direction given");
//				}

				poofPoofer (index, 0, value);

			} else {
				Debug.Log("Bad poofer index: "+message);
			}

		}
	}

	void createFireAncestor () {
		float xCurveOffset, zCurveOffset;
		float xCurveMagnitude = 0.5f;
		float zCurveMagnitude = 0.5f;
		for (int i = 0; i < layersCount; i++) {
			xCurveOffset = Mathf.Sin ((float)i / (float)layersCount * (Mathf.PI*3f)) * (xCurveMagnitude * (float)i);
			zCurveOffset = Mathf.Cos ((float)i / (float)layersCount * (Mathf.PI*3f)) * (zCurveMagnitude * (float)i);
			Debug.Log ("xCurveOffset = " + xCurveOffset);
			GameObject newLayer = Instantiate (layerPrefab, new Vector3 (i * xOffset + xCurveOffset, i * ySpacing, i * zOffset + zCurveOffset), Quaternion.Euler(0.0f, i * 15f, 0.0f), faParent.transform) as GameObject;
			LayerController newLayerController = newLayer.GetComponentInChildren<LayerController> ();
			layerControllers.Add (newLayerController);
		}

//		Debug.Log ("Layers = " + layers );
	}

	// Update is called once per frame
	void Update () {

		if (isOSCListning) {
			OscMessage tempMessage;
			while ((tempMessage = msgQueue.GetNextMessage()) != null) {
				handleOSCMessage(tempMessage);
			}
		}

		faParent.transform.Rotate (new Vector3 (0.0f, rotationSpeed, 0.0f));
	}
}
