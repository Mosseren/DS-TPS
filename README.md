# 动画序列分段
## 1. 前摇（Start）  
动画通知： UStartHitBoxNotify  
接受攻击输入：false  
进入该区域，设置标志位，bIsRotatingToControllerYaw，Tick检查该标志位为真则朝向控制器旋转，这样可以增加打击命中的准确性。  

## 2. 释放（Hit Box）  
动画通知： UStartHitBoxNotify、UEndHitBoxNotify  
接受攻击输入：false  
进入该区域，触发OnStartHitBox事件，将碰撞盒（HitBox）设置为启用，并开始检测与敌人的碰撞。  
离开该区域，关闭碰撞检测，防止持续伤害敌人。  

## 3. 连段（Combo）  
动画通知： UEndHitBoxNotify、UEndComboNotify  
接受攻击输入：true  
进入该区域，AttackIdx会增加，以便触发下一个攻击动画，形成连击效果。  
离开该区域，连段数置为0，表示错过连段时机。  

## 4. 后摇（Post Attack）  
动画通知： UEndComboNotify  
接受攻击输入：true  
在攻击完成后的后摇阶段，角色动作恢复到Idle。  

## 5. 被其他动画打断  
因为连段也算打断，因此在动画混合结束时检测正在播放的动画，如果不是攻击动画，就说明被其他动画打断。  
此时调用OnEndHitBox和OnEndCombo来结束当前攻击，避免无法攻击、持续伤害等bug。  


# 具体流程：  
AttackMontage的绑定： 在BeginPlay中绑定不同的动画通知，这样在攻击时，通过通知来激活不同的碰撞框和动作流程。  
输入处理： Fire函数处理攻击的输入，激活攻击动画并跳转到当前攻击的特定部分。通过Montage_JumpToSection来确保攻击动作的正确顺序。  
攻击输入控制： 使用bShouldHandleInput来控制攻击输入，确保输入在合适的时间进行处理。  
碰撞和伤害检测： OnBeginOverlapEnemy函数处理武器与敌人的碰撞，敌人被击中后触发伤害事件，调用TakeDamage函数来给敌人造成伤害。  