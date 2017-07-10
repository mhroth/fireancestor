using UnityEngine;
using System.Collections;
using System;
using SharpOSC;

public class FireAncestorController : MonoBehaviour {

	public GameObject layerPrefab;
	public int layersCount = 25;
	public float ySpacing = 7.2f;
	public float xOffset = -19.608333333f;
	public float zOffset = -11.608333333f;
	public LayerController[] layers = new LayerController[25];

	// Use this for initialization
	void Start () {

		createFireAncestor ();

		Application.runInBackground = true;

		HandleOscPacket callback = delegate(OscPacket packet) {

			var message = (OscMessage)packet;

			string type = message.Address;

			Debug.Log("Received poofer "+message+" "+type);

			if (type.StartsWith("/poofer_")) {

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

					float value = 0.0f;

					if (message.Arguments[0].GetType() == typeof(System.Single)) {
						value = (float)message.Arguments[0];
					} else if (message.Arguments[0].GetType() == typeof(System.Int32)) {
						value = (float)((int)message.Arguments[0]);
					}

					int index = Int32.Parse(suffix);

					Debug.Log("Received poofer "+index+" "+value);

					poofPoofer(index, value);
				}
			}

		};

		var listener = new UDPListener(55555, callback);

	}

	void poofPoofer (int index, float value) {
		Debug.Log("Received poof "+index+" "+value);

		LayerController layer = layers [index];

//		ParticleSystem poofer = layer.pooferCW;
//
//		int emit = Mathf.RoundToInt(value);
//
//		poofer.Emit (emit);

	}

	void createFireAncestor () {
		for (int i = 0; i < layersCount; i++) {
			LayerController newLayer = Instantiate (layerPrefab, new Vector3 (i * xOffset, i * ySpacing, i * zOffset), Quaternion.Euler(0.0f, i * 15f, 0.0f)) as LayerController;
			layers [i] = newLayer;
		}
	}

	// Update is called once per frame
	void Update () {
	
	}
}
