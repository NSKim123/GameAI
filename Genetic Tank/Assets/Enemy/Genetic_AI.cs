using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System.Runtime.InteropServices;


public struct STATE
{
    public Vector3 Pos;
    public Vector3 Dir;
    public int State;

};

public class Genetic_AI : MonoBehaviour
{
    [DllImport("GeneticDLL")]
    public static extern void Init();

    [DllImport("GeneticDLL")]
    public static extern void onRelease();

    [DllImport("GeneticDLL")]
    public static extern void onSetHeroInfo(Vector3 pos, int hp);

    [DllImport("GeneticDLL")]
    public static extern void onSetEnemyInfo(int hp);

    [DllImport("GeneticDLL")]
    public static extern STATE onGetState();

    [DllImport("GeneticDLL")]
    public static extern void onFrameMove();

    [DllImport("GeneticDLL")]
    public static extern int onGetMapData(int y, int x);


}
