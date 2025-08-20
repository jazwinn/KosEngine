using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.Versioning;
using System.Security.Policy;
using System.Text;
using System.Threading.Tasks;
using System.Xml;

public class ParticleEmitter : ScriptBase
{
    private uint EntityID;
    public override void Awake(uint id)
    {
        EntityID = id;
    }

    public override void Start()
    {
    }

    public override void Update()
    {
        InternalCall.m_InternalCallSpawnParticle(EntityID);
    }
}