using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class ObjectMove2 : MonoBehaviour
{
    public GameObject Prefab_bullet;
    Vector3 Dir;
    float speed = 0.01f;
    int nTime =1;

    int Obj_Ty = 0;
    int Obj1_Hp = 100;
    int Obj2_Hp = 100;
    Vector3 Obj1_Pos;
    Vector3 Obj2_Pos;

    /// 추가 변수는 아래에 추가할것
    /// 



    public bool MoveFollowTarget()
    {
        if (Obj1_Hp >= 50)
        {
            Dir = Obj2_Pos - gameObject.transform.position;
            Dir.Normalize();
            Quaternion Rot = Quaternion.LookRotation(Dir, new Vector3(0, 1, 0));
            gameObject.transform.localRotation = Rot; 
            gameObject.transform.position += Dir * speed;
            return true;
        }
        return false;
    }

    public bool MoveBackollowTarget()
    {
        if (Obj1_Hp < 50 && Obj1_Hp > 0)
        {
            Dir = Obj2_Pos - gameObject.transform.position;
            Dir.Normalize();
            Quaternion Rot = Quaternion.LookRotation(Dir, new Vector3(0, 1, 0));
            gameObject.transform.localRotation = Rot;
            gameObject.transform.position -= Dir * speed;
            return true;
        }
        return false;
    }

    
    public bool IsDead()
    {
        if (Obj1_Hp <= 0)
        {
            return false;
        }
        return true;
    }
    public bool AddBullet()
    {
        if (Obj1_Hp > 0)
        {
            if (nTime % 100 == 0)
            {
                GameObject bullet = GameObject.Instantiate(Prefab_bullet) as GameObject;
                bullet.GetComponent<BulletMove>().Dir = Dir;
                bullet.transform.parent = null;
                bullet.gameObject.layer = LayerMask.NameToLayer("Obj2_Bullet");
                bullet.transform.position = transform.position;
                nTime++;
            }
            return true;
        }
        return false;
    }


    /// 수정 하지 말것!
    /// ////////////////////////////////////////////////////////////////////////////////
    void Start()
    {
        Obj_Ty = 0;
        Obj1_Hp = 100;
        Obj2_Hp = 100;
        Obj1_Pos = new Vector3(0, 0, 0);
        Obj2_Pos = new Vector3(0, 0, 0);
        gameObject.transform.position = GlobalData.Obj1_Pos;
        if (gameObject.layer == LayerMask.NameToLayer("Obj1"))
        {
            Obj_Ty = 0;
            gameObject.transform.position = GlobalData.Obj1_Pos;
        }
        else
        {
            Obj_Ty = 1;
            gameObject.transform.position = GlobalData.Obj2_Pos;
        }

    }

    private void FixedUpdate()
    {
        nTime++;
        if (Obj_Ty == 0)
        {
            Obj1_Hp = GlobalData.Obj1_Hp;
            Obj2_Hp = GlobalData.Obj2_Hp;
            Obj1_Pos = GlobalData.Obj1_Pos;
            Obj2_Pos = GlobalData.Obj2_Pos;
        }
        else
        {
            Obj1_Hp = GlobalData.Obj2_Hp;
            Obj2_Hp = GlobalData.Obj1_Hp;
            Obj1_Pos = GlobalData.Obj2_Pos;
            Obj2_Pos = GlobalData.Obj1_Pos;
        }
    }
    /////////////////////////////////////////////////////////////////////////////
}
