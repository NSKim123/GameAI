using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class CreateWall : MonoBehaviour
{
    public GameObject Prefab_Wall;
    void Start()
    {
        Genetic_AI.Init();

        float Scale = 6;
        for (int x = 0; x<5;x++)
        {
            for(int y=0; y<5;y++)
            {
                if (Genetic_AI.onGetMapData(y, x) == 1)
                {
                    GameObject Wall = GameObject.Instantiate(Prefab_Wall) as GameObject;

                    Wall.transform.parent = null;
                    Wall.gameObject.layer = LayerMask.NameToLayer("Wall");
                    Wall.transform.localScale = new Vector3(Scale, Scale, Scale);
                    Wall.transform.position = new Vector3((y-2)* Scale, 0,(x-2)* Scale);
                }
            }
        }
      
    }



    void OnApplicationQuit()
    {
        Genetic_AI.onRelease();
    }

}

// Update is called once per frame


