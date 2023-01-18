using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Object_AI2 : MonoBehaviour
{
    private class MoveFollowTarget : Node
    {
        public ObjectMove2 obj
        {
            set { _obj = value; }
        }
        private ObjectMove2 _obj;
        public override bool Invoke()
        {
            return _obj.MoveFollowTarget();

        }
    }

    private class MoveBackollowTarget : Node
    {
        public ObjectMove2 obj
        {
            set { _obj = value; }
        }
        private ObjectMove2 _obj;
        public override bool Invoke()
        {
            return _obj.MoveBackollowTarget();

        }
    }

    private class IsDead : Node
    {
        public ObjectMove2 obj
        {
            set { _obj = value; }
        }
        private ObjectMove2 _obj;
        public override bool Invoke()
        {
            return _obj.IsDead();
        }
    }

    private class OnAttack : Node
    {
        public ObjectMove2 obj
        {
            set { _obj = value; }
        }
        private ObjectMove2 _obj;
        public override bool Invoke()
        {
            return _obj.AddBullet();
        }
    }

    private Sequence root = new Sequence();
    private Selector selector = new Selector();
    private Sequence seqMovingAttack = new Sequence();
    private Sequence seqDead = new Sequence();

    private MoveFollowTarget moveForTarget = new MoveFollowTarget();
    private MoveBackollowTarget moveBackForTarget = new MoveBackollowTarget();
    private OnAttack m_OnAttack = new OnAttack();
    private IsDead m_IsDead = new IsDead();

    private ObjectMove2 m_obj;
    private IEnumerator behaviorProcess;

    public void Init()
    {
        m_obj = gameObject.GetComponent<ObjectMove2>();

        root.AddChild(selector);


        selector.AddChild(seqDead);
        selector.AddChild(seqMovingAttack);

        moveForTarget.obj = m_obj;
        moveBackForTarget.obj = m_obj;
        m_OnAttack.obj = m_obj;
        m_IsDead.obj = m_obj;


        seqMovingAttack.AddChild(moveForTarget);
        seqMovingAttack.AddChild(m_OnAttack);
        seqMovingAttack.AddChild(moveBackForTarget);

        seqDead.AddChild(m_IsDead);

        behaviorProcess = BehaviorProcess();
        StartCoroutine(behaviorProcess);
    }

    public IEnumerator BehaviorProcess()
    {
        while (root.Invoke())
        {
            yield return new WaitForEndOfFrame();
        }
        Destroy(gameObject, 0.0f);
        Debug.Log("behavior process exit");
    }

    void Start()
    {
        Debug.Log("Start Tree");
        Init();
    }

    void OnApplicationQuit()
    {
        Debug.Log("End Tree");
    }



}
