using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class EnemyMove : MonoBehaviour
{
    public GameObject Prefab_bullet;


    int nTime = 0;
    void Start()
    {

        gameObject.transform.position = Genetic_AI.onGetState().Pos;
    }

    void FixedUpdate()
    {
        Genetic_AI.onFrameMove();

        Vector3 Dir = Genetic_AI.onGetState().Dir;
        Vector3 Pos = Genetic_AI.onGetState().Pos;
        int State = Genetic_AI.onGetState().State;
        if(State==4)
        {
            AddBullet(Dir);
        }
        Quaternion Rot = Quaternion.LookRotation(Dir, new Vector3(0, 1, 0));
        gameObject.transform.localRotation = Rot;
        gameObject.transform.position = Genetic_AI.onGetState().Pos;

        Genetic_AI.onSetEnemyInfo(HeroMove.EnemyHp);
        nTime++;
    }



    public void AddBullet(Vector3 Dir)
    {
        if (nTime % 50 == 0)
        {
            GameObject bullet = GameObject.Instantiate(Prefab_bullet) as GameObject;
            bullet.GetComponent<BulletMove>().Dir = Dir;
            bullet.transform.parent = null;
            bullet.gameObject.layer = LayerMask.NameToLayer("Enemy");
            bullet.transform.position = transform.position;
        }
    }
}
