using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class GlobalData : MonoBehaviour
{

    public static int Obj1_Hp = 100;
    public static int Obj2_Hp = 100;
    public static Vector3 Obj1_Pos;
    public static Vector3 Obj2_Pos;
    Vector3 Obj1_Pos_org;
    Vector3 Obj2_Pos_org;
    int ntime = 0;
    void Awake()
    {
        Obj1_Hp = 100;
        Obj2_Hp = 100;
        Obj1_Pos = new Vector3(Random.Range(-23.0f, 23.0f), 0, -21.63f);
        Obj2_Pos = new Vector3(Random.Range(-23.0f, 23.0f), 0, 21.63f);
    }

    // Update is called once per frame
    void FixedUpdate()
    {
        if(ntime%100==0)
        {
            Obj1_Pos_org = GameObject.Find("Obj1").transform.position;
            Obj2_Pos_org = GameObject.Find("Obj2").transform.position;
            Obj1_Pos = Obj1_Pos_org + new Vector3(Random.Range(-3.0f, 3.0f), 0, Random.Range(-3.0f, 3.0f));
            Obj2_Pos = Obj2_Pos_org + new Vector3(Random.Range(-3.0f, 3.0f), 0, Random.Range(-3.0f, 3.0f));
        }
        
        ntime++;
    }

    void OnGUI()//소스로 GUI 표시.
    {
        
        GUIStyle style;
        Rect rect;
        int w = Screen.width, h = Screen.height;
        rect = new Rect(0, 0, w, h * 4 / 100);
        style = new GUIStyle();
        style.alignment = TextAnchor.UpperLeft;
        style.fontSize = h * 4 / 100;
        style.normal.textColor = Color.black;
       
        string text = "Obj1 HP: "+ Obj1_Hp + "% \nObj2 HP: "+ Obj2_Hp + "%";
        GUI.Label(rect, text, style);
    }

}
