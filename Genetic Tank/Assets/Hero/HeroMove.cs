using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class HeroMove : MonoBehaviour
{

    public static int HeroHp = 100;
    public static int EnemyHp = 100;
    public static Vector3 HeroPos;

    public GameObject Prefab_bullet;
    float DirR = 0;
    // Start is called before the first frame update
    void Start()
    {
        HeroPos = gameObject.transform.position;
        Genetic_AI.onSetHeroInfo(HeroPos, HeroHp);
    }

    // Update is called once per frame
    void FixedUpdate()
    {
        
        float speed = 0.1f;
       
        if (Input.GetKey(KeyCode.UpArrow))
        {
        
            gameObject.transform.Translate(Vector3.forward * speed);
        }

        if (Input.GetKey(KeyCode.DownArrow))
        {
           
            gameObject.transform.Translate(-Vector3.forward * speed);
        }
        if (Input.GetKey(KeyCode.LeftArrow))
        {
           
            gameObject.transform.Rotate(0, -1.0f, 0);
        }
        if (Input.GetKey(KeyCode.RightArrow))
        {
     
            gameObject.transform.Rotate(0, 1.0f, 0);
        }
        DirR = gameObject.transform.localEulerAngles.y;
        Vector3 Dir = new Vector3(Mathf.Sin(DirR / 180.0f * 3.14f), 0, Mathf.Cos(DirR / 180.0f * 3.14f));
        HeroPos = gameObject.transform.position;
        Genetic_AI.onSetHeroInfo(HeroPos, HeroHp);

    }
    void Update()
    {
        DirR = gameObject.transform.localEulerAngles.y;
        Vector3 Dir = new Vector3(Mathf.Sin(DirR / 180.0f * 3.14f), 0, Mathf.Cos(DirR / 180.0f * 3.14f));
        if (Input.GetKeyDown(KeyCode.Space))
        {
            GameObject bullet = GameObject.Instantiate(Prefab_bullet) as GameObject;
            bullet.GetComponent<BulletMove>().Dir = Dir;
            bullet.transform.parent = null;
            bullet.gameObject.layer = LayerMask.NameToLayer("Hero");
            bullet.transform.position = transform.position;
        }
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
       
        string text = "Hero HP: "+ HeroHp +"% \nEnemy HP: "+EnemyHp+"%";
        GUI.Label(rect, text, style);
    }

}
