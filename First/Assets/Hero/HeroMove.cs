using System.Collections;
using System.Collections.Generic;
using System.Collections.Specialized;
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
        HeroHp = 100;
        EnemyHp = 100;
        HeroPos = gameObject.transform.position;
    }

    // Update is called once per frame
    void Update()
    {
        Vector3 Dir = new Vector3(Mathf.Sin(DirR / 180.0f * 3.14f), 0, Mathf.Cos(DirR / 180.0f * 3.14f));

        float speed = 0.1f;

        if(Input.GetKey(KeyCode.UpArrow))
        {
            gameObject.transform.position += Dir * speed;
        }

        if (Input.GetKey(KeyCode.DownArrow))
        {
            gameObject.transform.position -= Dir * speed;
        }

        if (Input.GetKey(KeyCode.LeftArrow))
        {
            DirR -= 1.0f;
        }

        if (Input.GetKey(KeyCode.RightArrow))
        {
            DirR += 1.0f;
        }

        gameObject.transform.rotation = Quaternion.Euler(0, DirR, 0);
        HeroPos = gameObject.transform.position;

        if(Input.GetKeyDown(KeyCode.Space))
        {
            if (Prefab_bullet != null)
            {
                GameObject bullet = GameObject.Instantiate(Prefab_bullet) as GameObject;
                bullet.GetComponent<BulletMove>().Dir = Dir;
                bullet.transform.parent = null;
                bullet.gameObject.layer = LayerMask.NameToLayer("Hero");
                bullet.transform.position = transform.position;
            }
        }
    }

    void OnGUI()
    {
        GUIStyle style;
        Rect rect;
        int w = Screen.width, h = Screen.height;
        rect = new Rect(0, 0, w, h * 4 / 100);
        style = new GUIStyle();
        style.alignment = TextAnchor.UpperLeft;
        style.fontSize = h * 4 / 100;
        style.normal.textColor = Color.black;

        string text = "Hero : HP: " + HeroHp + "% \nEnemy HP: " + EnemyHp + "%";
        GUI.Label(rect,text, style);
    }
}
