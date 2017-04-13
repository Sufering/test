// 49028 - Dancing Rune Weapon
class spell_dk_dancing_rune_weapon : public SpellScriptLoader
{
    public:
        spell_dk_dancing_rune_weapon() : SpellScriptLoader("spell_dk_dancing_rune_weapon") { }

        class spell_dk_dancing_rune_weapon_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_dk_dancing_rune_weapon_AuraScript);

            bool Validate(SpellInfo const* /*spellInfo*/) override
            {
                if (!sObjectMgr->GetCreatureTemplate(NPC_DK_DANCING_RUNE_WEAPON))
                    return false;
                return true;
            }

            // Isso e uma gambiarra, nao e correto 
            void HandleProc(AuraEffect const* /*aurEff*/, ProcEventInfo& eventInfo)
            {
                PreventDefaultAction();
                Unit* caster = GetCaster();
                if (!caster)
                    return;

                Unit* drw = nullptr;
                for (Unit* controlled : caster->m_Controlled)
                {
                    if (controlled->GetEntry() == NPC_DK_DANCING_RUNE_WEAPON)
                    {
                        drw = controlled;
                        break;
                    }
                }

                if (!drw || !drw->GetVictim())
                    return;

                SpellInfo const* spellInfo = eventInfo.GetSpellInfo();
                if (!spellInfo)
                    return;

                DamageInfo* damageInfo = eventInfo.GetDamageInfo();
                if (!damageInfo || !damageInfo->GetDamage())
                    return;

                int32 amount = static_cast<int32>(damageInfo->GetDamage()) / 2;
                drw->SendSpellNonMeleeDamageLog(drw->GetVictim(), spellInfo->Id, amount, spellInfo->GetSchoolMask(), 0, 0, false, 0, false);
                drw->DealDamage(drw->GetVictim(), amount, nullptr, SPELL_DIRECT_DAMAGE, spellInfo->GetSchoolMask(), spellInfo, true);
            }

            void Register() override
            {
                OnEffectProc += AuraEffectProcFn(spell_dk_dancing_rune_weapon_AuraScript::HandleProc, EFFECT_1, SPELL_AURA_DUMMY);
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new spell_dk_dancing_rune_weapon_AuraScript();
        }
