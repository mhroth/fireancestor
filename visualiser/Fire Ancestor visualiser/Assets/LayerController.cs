using UnityEngine;
using System.Collections;
using System;

public class LayerController : MonoBehaviour {

	public ParticleSystem pooferCW;
	public ParticleSystem pooferCCW;
	private int t = 0;

	// Use this for initialization
	void Start () {


	}
	
	// Update is called once per frame
	void Update () {
		if ((t % 120) == 0) {
			
//			pooferCW.Emit (2000);

		} else if ((t % 120) == 60) {
			
//			pooferCCW.Emit (2000);

		}
		t++;
	}
}
