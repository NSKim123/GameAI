using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public abstract class Node
{
    public abstract bool Invoke();
}

public class CompositeNode : Node
{
    public override bool Invoke()
    {
        throw new NotImplementedException();
    }

    public void AddChild(Node node)
    {
        childrens.Push(node);
    }
    public Stack<Node> GetChildrens()
    {
        return childrens;
    }
    private Stack<Node> childrens = new Stack<Node>();
}

public class Selector : CompositeNode
{
    public override bool Invoke()
    {
        foreach(var node in GetChildrens())
        {
            if(node.Invoke())
            {
                return true; 
            }
        }
        return false;
    }
}

public class Sequence : CompositeNode
{
    public override bool Invoke()
    {
        bool p = false;
        foreach(var node in GetChildrens())
        {
            if(node.Invoke() == false)
            {
                p = true;
            }
            
        }
        return !p;
    }
    
}

public class MoveFollowTarget : Node
{
    public EnemyMove Enemy
    {
        set { _Enemy = value; }
            
    }
    private EnemyMove _Enemy;
    public override bool Invoke()
    {
        return _Enemy.MoveFollowTarget();
    }
}

public class MoveBackollowTarget : Node
{
    public EnemyMove Enemy
    {
        set { _Enemy = value; }

    }
    private EnemyMove _Enemy;
    public override bool Invoke()
    {
        return _Enemy.MoveBackollowTarget();
    }
}

public class IsDead : Node
{
    public EnemyMove Enemy
    {
        set { _Enemy = value; }

    }
    private EnemyMove _Enemy;
    public override bool Invoke()
    {
        return _Enemy.IsDead();
    }
}

public class OnAttack : Node
{
    public EnemyMove Enemy
    {
        set { _Enemy = value; }

    }
    private EnemyMove _Enemy;
    public override bool Invoke()
    {
        return _Enemy.AddBullet();
    }
}
