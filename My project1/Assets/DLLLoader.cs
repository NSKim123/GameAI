using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System.Runtime.InteropServices;

public class DLLLoader : MonoBehaviour
{
    [DllImport("TestDLL")]
    public static extern void Init();
    [DllImport("TestDLL")]
    public static extern void onRelease();
    [DllImport("TestDLL")]
    public static extern int GetState();

    // Start is called before the first frame update
    void Start()
    {        
        Init();
    }   
    void FixedUpdate()
    {
        int state = GetState();    
    }
    private void OnApplicationQuit()
    {
        onRelease();
    }
}
