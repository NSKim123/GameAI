using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class ObjectMove_KNS : MonoBehaviour
{
    public GameObject Prefab_bullet;
    Vector3 Dir;
    float speed = 0.01f;
    int nTime = 1;

    int Obj_Ty = 0;
    int Obj1_Hp = 100;
    int Obj2_Hp = 100;
    Vector3 Obj1_Pos;
    Vector3 Obj2_Pos;

    /// 추가 변수는 아래에 추가할것
    /// 

    private bool follow = true;
    private Vector3 Obj2_Pre_Pos = Vector3.zero;
    private Vector3 Obj2_Delta_Esti = Vector3.zero;
    private Vector3 Obj2_Delta_Pos = Vector3.zero;
    

    private float onRBE(float x, float beforeDir)
    {
        float p1 = x * x;
        float p2 = p1 / 2;

        float RBE = Mathf.Exp(-p1 / (p2 * p2));

        if (p1 == 0)
            RBE = 0;

        float dir = x * RBE;

        return beforeDir * 0.5f + dir * 0.5f;
    }    

    public bool MoveFollowTarget()
    {
        if (follow)
        {            
            
            Dir = Obj2_Pos - gameObject.transform.position + Obj2_Delta_Esti;
            Dir.Normalize();            
            Quaternion Rot = Quaternion.LookRotation(Dir, new Vector3(0, 1, 0));            
            gameObject.transform.localRotation = Rot;                 
            gameObject.transform.position += Dir * speed;

            if(nTime % 100 <= 1)
            {                
                if(Obj2_Pre_Pos != Vector3.zero)
                {
                    Obj2_Delta_Pos = Obj2_Pos - Obj2_Pre_Pos;

                    Obj2_Delta_Esti.x = onRBE(Obj2_Delta_Pos.x, Obj2_Delta_Esti.x);
                    Obj2_Delta_Esti.z = onRBE(Obj2_Delta_Pos.z, Obj2_Delta_Esti.z);
                }    


                Obj2_Pre_Pos = Obj2_Pos;

                if (Vector3.Distance(Obj1_Pos, Obj2_Pos) < 30.0f)
                {
                    follow = false;
                }
                if (Vector3.Distance(Obj1_Pos, Obj2_Pos) >= 30.0f)
                {
                    follow = true;
                }
            }

            return true;
        }        

        return false;
    }

    public bool MoveBackollowTarget()
    {
        if (!follow)
        {            
            Dir = Obj2_Pos - gameObject.transform.position + Obj2_Delta_Esti;
            Dir.Normalize();
            Dir = Quaternion.Euler(0, 25, 0) * Dir;
            Quaternion Rot = Quaternion.LookRotation(Dir, new Vector3(0, 1, 0));

            gameObject.transform.localRotation = Rot;
            gameObject.transform.position -= Dir * speed;

            if (nTime % 100 <= 1)
            {
                Obj2_Delta_Pos = Obj2_Pos - Obj2_Pre_Pos;
                
                Obj2_Delta_Esti.x = onRBE(Obj2_Delta_Pos.x, Obj2_Delta_Esti.x);
                Obj2_Delta_Esti.z = onRBE(Obj2_Delta_Pos.z, Obj2_Delta_Esti.z);

                Obj2_Pre_Pos = Obj2_Pos;

                if (Vector3.Distance(Obj1_Pos, Obj2_Pos) < 30.0f)
                {
                    follow = false;
                }
                if (Vector3.Distance(Obj1_Pos, Obj2_Pos) >= 30.0f)
                {
                    follow = true;
                }
            }

            return true;
        }
        

        return false;
    }

    public bool IsDead()
    {
        if (Obj1_Hp  <= 0)
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
                bullet.gameObject.layer = LayerMask.NameToLayer("Obj1_Bullet");
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
