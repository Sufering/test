  // Spell Dancing rune weapom 
            if (dummySpell->Id == 49028)
            {
                //Aura para remover
                if (effIndex != 1)
                    return false;

                Unit* pPet = NULL;
                for (ControlList::const_iterator itr = m_Controlled.begin(); itr != m_Controlled.end(); ++itr) // Find Rune Weapon
                    if ((*itr)->GetEntry() == 27893)
                    {
                        pPet = *itr;
                        break;
                    }

                //Dano das habilidades
                if (pPet && pPet->getVictim() && damage && procSpell)
                {
                    pPet->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                    pPet->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                    uint32 procDmg = damage / 2;
                    pPet->SendSpellNonMeleeDamageLog(pPet->getVictim(), procSpell->Id, procDmg, GetSpellSchoolMask(procSpell), 0, 0, false, 0, false);
                    pPet->DealDamage(pPet->getVictim(), procDmg, NULL, SPELL_DIRECT_DAMAGE, GetSpellSchoolMask(procSpell), procSpell, true);
                    break;
                }
                else //Copiar 50% do dano melee
                if (pPet && pPet->getVictim() && damage && !procSpell)
                {
                        CalcDamageInfo damageInfo;
                        CalculateMeleeDamage(pPet->getVictim(), 0, &damageInfo, BASE_ATTACK);
                        damageInfo.attacker = pPet;
                        damageInfo.damage = damageInfo.damage / 2;
                        //Log de dano do cliente
                        pPet->DealDamageMods(pPet->getVictim(),damageInfo.damage,&damageInfo.absorb);
                        pPet->SendAttackStateUpdate(&damageInfo);
                        pPet->ProcDamageAndSpell(damageInfo.target, damageInfo.procAttacker, damageInfo.procVictim, damageInfo.procEx, damageInfo.damage, damageInfo.attackType);
                        pPet->DealMeleeDamage(&damageInfo,true);
                }
                else
                    return false;
            }
