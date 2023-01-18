using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class BulletMove : MonoBehaviour
{
    // Start is called before the first frame update
    public Vector3 Dir;
    void Start()
    {
        Destroy(gameObject, 5.0f);
    }

    // Update is called once per frame
    void FixedUpdate()
    {
        gameObject.transform.position += Dir * 0.5f;
     
    }
    void OnTriggerEnter(Collider col)
    {
        if (col.gameObject.layer == LayerMask.NameToLayer("Obj2") && gameObject.layer == LayerMask.NameToLayer("Obj1_Bullet"))
        {
           GlobalData.Obj2_Hp-=5;
           Destroy(gameObject, 0.0f);
            
        }

        if (col.gameObject.layer == LayerMask.NameToLayer("Obj1") && gameObject.layer == LayerMask.NameToLayer("Obj2_Bullet"))
        {
            GlobalData.Obj1_Hp -= 5;
            Destroy(gameObject, 0.0f);

        }
    }
}
