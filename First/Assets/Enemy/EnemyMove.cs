using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.Rendering;

public class EnemyMove : MonoBehaviour
{
    public GameObject Prefab_bullet;
    float DirR = 180.0f;
    Vector3 Dir;
    float speed = 0.02f;
    int nTime = 0;
    
    public bool MoveFollowTarget()
    {
        if(HeroMove.EnemyHp >= 50)
        {
            Dir = HeroMove.HeroPos - gameObject.transform.position;
            Dir.Normalize();
            Quaternion Rot = Quaternion.LookRotation(Dir, new Vector3(0, 1, 0)); 
            DirR = Rot.eulerAngles.y;
            gameObject.transform.localRotation = Rot;
            gameObject.transform.position += Dir * speed;
            return true;
        }
        return false;
    }

    public bool MoveBackollowTarget()
    {
        if(HeroMove.EnemyHp < 50 && HeroMove.EnemyHp>0)
        {
            Dir= HeroMove.HeroPos - gameObject.transform.position;
            Dir.Normalize();
            Quaternion Rot = Quaternion.LookRotation(Dir, new Vector3(0,1,0));
            DirR = Rot.eulerAngles.y;
            gameObject.transform.localRotation = Rot;
            gameObject.transform.position -= Dir * speed;
            return true;
        }
        return false;
    }

    void Update()
    {
        nTime++;
    }

    public bool IsDead()
    {
        if(HeroMove.EnemyHp <= 0)
        {
            Destroy(gameObject, 0.0f);
            return false;
        }
        return true;
    }
    public bool AddBullet()
    {
        if(HeroMove.EnemyHp> 0) 
        {
            if (nTime %100 ==0)
            {
                GameObject bullet = GameObject.Instantiate(Prefab_bullet) as GameObject;
                bullet.GetComponent<BulletMove>().Dir = Dir;
                bullet.transform.parent = null;
                bullet.gameObject.layer = LayerMask.NameToLayer("Enemy");
                bullet.transform.position = transform.position;
            }    
            return true;
        }
        return false;
    }
}
