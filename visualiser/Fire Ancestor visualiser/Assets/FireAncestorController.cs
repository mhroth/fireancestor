using UnityEngine;
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
//	public GameObject[] layers;
	private List<GameObject> layers = new List<GameObject>();
	private List<LayerController> layerControllers = new List<LayerController>();
//	private LayerController[] layerControllers = new LayerController[25];
	private ParticleSystem poofer;
	private bool isOSCListning = false;
	private List<OscMessage> messageQueue = new List<OscMessage> ();

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


	void poofPoofer (int index, float value) {

		Debug.Log("Received poofer "+index+" "+value);

		LayerController layerController = layerControllers[index];

		poofer = layerController.pooferCW;

		int emit = Mathf.RoundToInt(value);

		poofer.Emit (emit);

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

				poofPoofer (index, value);

			} else {
				Debug.Log("Bad poofer index: "+message);
			}

		}
	}

	void createFireAncestor () {
		for (int i = 0; i < layersCount; i++) {
//			layers[i] = Instantiate (layerPrefab, new Vector3 (i * xOffset, i * ySpacing, i * zOffset), Quaternion.Euler(0.0f, i * 15f, 0.0f)) as LayerController;

			GameObject newLayer = Instantiate (layerPrefab, new Vector3 (i * xOffset, i * ySpacing, i * zOffset), Quaternion.Euler(0.0f, i * 15f, 0.0f)) as GameObject;
			LayerController newLayerController = newLayer.GetComponentInChildren<LayerController> ();
			layerControllers.Add (newLayerController);
//			layerControllers [i] = newLayerController;
//			Debug.Log ("Layersaa = " + layerControllers[i] );
		}

		Debug.Log ("Layers = " + layers );
	}

	// Update is called once per frame
	void Update () {
//		if (isOSCListning) {
//			FireAncestorController.FloatMessage tempMessage;
//			while ((tempMessage = msgQueue.GetNextMessage()) != null) {
//				FloatReceivedCallback(tempMessage);
//			}
//		}

		if (isOSCListning) {
			OscMessage tempMessage;
			while ((tempMessage = msgQueue.GetNextMessage()) != null) {
				handleOSCMessage(tempMessage);
			}
		}
	}


//
//	[MonoPInvokeCallback(typeof(SendHook))]
//	private static void OnMessageSent(IntPtr context, string sendName, uint sendHash, IntPtr message) {
//		if (hv_msg_hasFormat(message, "f")) {
//			SendMessageQueue msgQueue = (SendMessageQueue) GCHandle.FromIntPtr(hv_getUserData(context)).Target;
//			msgQueue.AddMessage(sendName, hv_msg_getFloat(message, 0));
//		}
//	}
}
