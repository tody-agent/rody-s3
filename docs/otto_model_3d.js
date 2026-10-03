/**
 * Otto S3 / HP Robots Otto Display 3D Robot Model for Three.js
 * 
 * 100% Faithful to Official HP Robots Otto Starter with Front Emotion Screen (Printables #829419)
 * Official CAD Assemblies (Top, Middle, Display Face, Bottom, Wheels)
 * Sharp Unwarped Hard-Surface Shading (Non-Indexed Face Normals)
 * High-Definition Animated Emotion Screen & Rigged Differential Drive Kinematics.
 */

(function (root, factory) {
  if (typeof define === 'function' && define.amd) {
    define(['three'], factory);
  } else if (typeof module === 'object' && module.exports) {
    module.exports = factory(require('three'));
  } else {
    root.OttoRobot3D = factory(root.THREE);
  }
}(typeof self !== 'undefined' ? self : this, function (THREE) {
  'use strict';

  const T = THREE || (typeof window !== 'undefined' ? window.THREE : null);

  // Physical dimensions in cm (1 unit = 10mm = 1cm)
  const DIM = {
    CHASSIS_W: 7.2,
    CHASSIS_D: 7.2,
    CHASSIS_H: 8.2,
    TRACK_WIDTH: 8.12,
    WHEEL_RADIUS: 1.95,
    AXLE_Y: 3.164, // Height of wheel bottom to axle
    CAD_SCALE: 0.1 // 1mm -> 0.1cm
  };

  class OttoRobot3D {
    /**
     * @param {Object} options
     * @param {string} [options.theme='white'] - 'white' | 'graphite' | 'mint' | 'yellow'
     * @param {boolean} [options.enableLights=true] - Cast glow from headlights/screen
     * @param {string} [options.modelsPath='models/otto_starter/'] - Directory containing official GLBs
     */
    constructor(options = {}) {
      if (!T) {
        throw new Error('Three.js must be loaded before instantiating OttoRobot3D');
      }

      this.options = Object.assign({
        theme: 'white',
        enableLights: true,
        modelsPath: 'models/otto_starter/'
      }, options);

      // Root scene container
      this.root = new T.Group();
      this.root.name = 'OttoRobot_Root';

      // Kinematic state
      this.speedL = 0;       // -100 to +100
      this.speedR = 0;       // -100 to +100
      this.wheelAngleL = 0;
      this.wheelAngleR = 0;
      this.currentPitch = 0;
      this.targetPitch = 0;
      this.currentRoll = 0;
      this.targetRoll = 0;
      this.idleTime = 0;

      // Emotion & Screen state machine
      this.currentEmotion = 'idle';
      this.eyeBlink = 0;
      this.nextBlinkTime = 2.0;
      this.pupilX = 0;
      this.pupilY = 0;
      this.screenNeedsRedraw = true;
      this.bodyParts = [];

      // Kinematic hierarchy
      // chassisGroup holds body + pivots, elevated so wheels touch Y = 0
      this.chassisGroup = new T.Group();
      this.chassisGroup.name = 'chassisGroup';
      this.chassisGroup.position.y = DIM.AXLE_Y;
      this.root.add(this.chassisGroup);

      // Independent Wheel Pivots for Differential Drive
      // Axle centers in cm: X = +/-4.0612, Y = -1.0, Z = -2.3
      this.leftWheelPivot = new T.Group();
      this.leftWheelPivot.name = 'leftWheelPivot';
      this.leftWheelPivot.position.set(-4.0612, -1.0, -2.3);
      this.chassisGroup.add(this.leftWheelPivot);

      this.rightWheelPivot = new T.Group();
      this.rightWheelPivot.name = 'rightWheelPivot';
      this.rightWheelPivot.position.set(4.0612, -1.0, -2.3);
      this.chassisGroup.add(this.rightWheelPivot);

      // Build components
      this._initMaterials();
      this._initEmotionScreen();
      this._buildProceduralFallback();
      this._loadOfficialCADModel();
      this.setChassisColor(this.options.theme);
    }

    /* -------------------------------------------------------------
     * 1. Official Materials Palette (Sharp Hard-Surface PBR)
     * ----------------------------------------------------------- */
    _initMaterials() {
      // Top: Crisp White PLA (#FFFFFF)
      this.matTop = new T.MeshStandardMaterial({
        color: 0xffffff,
        roughness: 0.32,
        metalness: 0.04,
        side: T.DoubleSide
      });

      // Middle: Official HP Robotic Sky Blue (#549EF7)
      this.matMiddle = new T.MeshStandardMaterial({
        color: 0x549ef7,
        roughness: 0.35,
        metalness: 0.05,
        side: T.DoubleSide
      });

      // Faceplate: Official HP Robotic Sky Blue (#549EF7)
      this.matFace = new T.MeshStandardMaterial({
        color: 0x549ef7,
        roughness: 0.35,
        metalness: 0.05,
        side: T.DoubleSide
      });

      // Bottom: Crisp White PLA (#FFFFFF)
      this.matBottom = new T.MeshStandardMaterial({
        color: 0xffffff,
        roughness: 0.35,
        metalness: 0.04,
        side: T.DoubleSide
      });

      // Wheel Tire: Dark Charcoal Rubber (#1F242D)
      this.matTire = new T.MeshStandardMaterial({
        color: 0x1f242d,
        roughness: 0.85,
        metalness: 0.08,
        side: T.DoubleSide
      });

      // Caster Ball: Polished Chrome Silver
      this.matChrome = new T.MeshStandardMaterial({
        color: 0xe2e8f0,
        roughness: 0.15,
        metalness: 0.90,
        side: T.DoubleSide
      });
    }

    /* -------------------------------------------------------------
     * 2. High-Definition Emotion Screen (Dynamic Vector Canvas)
     * ----------------------------------------------------------- */
    _initEmotionScreen() {
      this.screenCanvas = document.createElement('canvas');
      this.screenCanvas.width = 512;
      this.screenCanvas.height = 320;
      this.sCtx = this.screenCanvas.getContext('2d');

      this.screenTex = new T.CanvasTexture(this.screenCanvas);
      this.screenTex.generateMipmaps = false;
      this.screenTex.minFilter = T.LinearFilter;
      this.screenTex.magFilter = T.LinearFilter;

      // Screen material with emissive backlight
      this.matScreen = new T.MeshStandardMaterial({
        map: this.screenTex,
        emissiveMap: this.screenTex,
        emissive: 0xffffff,
        emissiveIntensity: 0.95,
        roughness: 0.25,
        metalness: 0.1
      });

      // Protective glossy screen glass
      this.matGlass = new T.MeshPhysicalMaterial({
        color: 0xffffff,
        transparent: true,
        opacity: 0.18,
        roughness: 0.04,
        metalness: 0.1,
        clearcoat: 1.0,
        clearcoatRoughness: 0.04
      });

      this._redrawScreen();
    }

    _redrawScreen() {
      const w = this.screenCanvas.width;
      const h = this.screenCanvas.height;
      const ctx = this.sCtx;

      ctx.clearRect(0, 0, w, h);

      // Deep OLED curved background
      const bgGrad = ctx.createRadialGradient(w/2, h/2, 20, w/2, h/2, w/1.2);
      bgGrad.addColorStop(0, '#0a1628');
      bgGrad.addColorStop(1, '#030712');
      ctx.fillStyle = bgGrad;
      ctx.fillRect(0, 0, w, h);

      // Eye Centers
      const eyeSpacing = 120;
      const leftX = w / 2 - eyeSpacing + this.pupilX;
      const rightX = w / 2 + eyeSpacing + this.pupilX;
      const centerY = h / 2 + this.pupilY;
      const blink = this.eyeBlink;

      if (this.currentEmotion === 'happy') {
        // Cheerful crescent upward curve eyes ^ ^
        ctx.strokeStyle = '#34d399';
        ctx.lineWidth = 26;
        ctx.lineCap = 'round';
        ctx.shadowColor = '#10b981';
        ctx.shadowBlur = 30;

        [-eyeSpacing, eyeSpacing].forEach(off => {
          ctx.beginPath();
          ctx.arc(w/2 + off, centerY + 20, 52, Math.PI * 1.15, Math.PI * 1.85);
          ctx.stroke();
        });
      } else if (this.currentEmotion === 'dizzy') {
        // Swirly cartoon spiral eyes @ @
        ctx.strokeStyle = '#fbbf24';
        ctx.lineWidth = 14;
        ctx.lineCap = 'round';
        ctx.shadowColor = '#f59e0b';
        ctx.shadowBlur = 25;

        [-eyeSpacing, eyeSpacing].forEach(off => {
          const cx = w/2 + off;
          ctx.beginPath();
          for (let a = 0; a < Math.PI * 4; a += 0.15) {
            const r = 6 + a * 9;
            const x = cx + Math.cos(a + performance.now() * 0.01) * r;
            const y = centerY + Math.sin(a + performance.now() * 0.01) * r;
            if (a === 0) ctx.moveTo(x, y);
            else ctx.lineTo(x, y);
          }
          ctx.stroke();
        });
      } else if (this.currentEmotion === 'obstacle') {
        // Alert wide warning eyes ! ! with ruby glow
        ctx.shadowColor = '#ef4444';
        ctx.shadowBlur = 35;
        ctx.fillStyle = '#f87171';

        [-eyeSpacing, eyeSpacing].forEach(off => {
          const cx = w/2 + off;
          ctx.beginPath();
          ctx.roundRect(cx - 36, centerY - 55, 72, 80, 24);
          ctx.fill();

          ctx.fillStyle = '#ffffff';
          ctx.beginPath();
          ctx.roundRect(cx - 10, centerY - 45, 20, 40, 8);
          ctx.fill();

          ctx.beginPath();
          ctx.arc(cx, centerY + 12, 10, 0, Math.PI * 2);
          ctx.fill();

          ctx.fillStyle = '#f87171';
        });
      } else if (this.currentEmotion === 'drive_fwd') {
        // Sporty forward racing slant eyes > <
        ctx.strokeStyle = '#00f0ff';
        ctx.lineWidth = 24;
        ctx.lineCap = 'round';
        ctx.shadowColor = '#0284c7';
        ctx.shadowBlur = 32;

        // Left eye >
        ctx.beginPath();
        ctx.moveTo(leftX - 35, centerY - 35);
        ctx.lineTo(leftX + 25, centerY);
        ctx.lineTo(leftX - 35, centerY + 35);
        ctx.stroke();

        // Right eye <
        ctx.beginPath();
        ctx.moveTo(rightX + 35, centerY - 35);
        ctx.lineTo(rightX - 25, centerY);
        ctx.lineTo(rightX + 35, centerY + 35);
        ctx.stroke();
      } else if (this.currentEmotion === 'sleepy') {
        // Resting sleepy horizontal slits - -
        ctx.strokeStyle = '#818cf8';
        ctx.lineWidth = 14;
        ctx.lineCap = 'round';
        ctx.shadowColor = '#6366f1';
        ctx.shadowBlur = 20;

        [-eyeSpacing, eyeSpacing].forEach(off => {
          ctx.beginPath();
          ctx.moveTo(w/2 + off - 36, centerY);
          ctx.lineTo(w/2 + off + 36, centerY);
          ctx.stroke();
        });
      } else {
        // IDLE / NORMAL: Gorgeous anime robot pill eyes with specular gloss
        const eyeW = 72;
        const eyeH = 100 * (1 - blink);
        const eyeR = 36 * (1 - blink);

        ctx.shadowColor = '#00f0ff';
        ctx.shadowBlur = 35;
        ctx.fillStyle = '#22d3ee';

        const drawEye = (x, y) => {
          if (eyeH < 6) {
            ctx.strokeStyle = '#38bdf8';
            ctx.lineWidth = 10;
            ctx.lineCap = 'round';
            ctx.beginPath();
            ctx.moveTo(x - eyeW/2, y);
            ctx.lineTo(x + eyeW/2, y);
            ctx.stroke();
            return;
          }

          ctx.beginPath();
          ctx.roundRect(x - eyeW/2, y - eyeH/2, eyeW, eyeH, Math.min(eyeR, eyeH/2));
          ctx.fill();

          // Main glossy pupil highlight
          ctx.shadowBlur = 0;
          ctx.fillStyle = '#ffffff';
          ctx.beginPath();
          ctx.arc(x - 14, y - eyeH/4, 14 * (1 - blink), 0, Math.PI * 2);
          ctx.fill();

          // Secondary micro-sparkle
          ctx.beginPath();
          ctx.arc(x + 16, y + eyeH/6, 7 * (1 - blink), 0, Math.PI * 2);
          ctx.fill();
        };

        drawEye(leftX, centerY);
        drawEye(rightX, centerY);
      }

      this.screenTex.needsUpdate = true;
      this.screenNeedsRedraw = false;
    }

    /* -------------------------------------------------------------
     * 3. Procedural Fallback Placeholder (while loading GLTF)
     * ----------------------------------------------------------- */
    _buildProceduralFallback() {
      this.fallbackGroup = new T.Group();
      this.fallbackGroup.name = 'fallbackGroup';

      const bodyGeo = new T.BoxGeometry(7.0, 7.2, 6.8);
      const bodyMesh = new T.Mesh(bodyGeo, this.matMiddle);
      bodyMesh.position.set(0, 0.4, 0);
      bodyMesh.castShadow = true;
      this.fallbackGroup.add(bodyMesh);

      const topGeo = new T.BoxGeometry(7.05, 1.8, 6.85);
      const topMesh = new T.Mesh(topGeo, this.matTop);
      topMesh.position.set(0, 3.2, 0);
      topMesh.castShadow = true;
      this.fallbackGroup.add(topMesh);

      const botGeo = new T.BoxGeometry(6.9, 1.6, 6.7);
      const botMesh = new T.Mesh(botGeo, this.matBottom);
      botMesh.position.set(0, -2.4, 0);
      botMesh.castShadow = true;
      this.fallbackGroup.add(botMesh);

      this.chassisGroup.add(this.fallbackGroup);
    }

    /* -------------------------------------------------------------
     * 4. Load Official HP Robots CAD Parts with Sharp Normals
     * ----------------------------------------------------------- */
    _loadOfficialCADModel() {
      if (typeof T.GLTFLoader === 'undefined') {
        console.warn('THREE.GLTFLoader not available; running procedural fallback.');
        return;
      }

      const loader = new T.GLTFLoader();
      const basePath = this.options.modelsPath || 'models/otto_starter/';

      // Notice: Uses face_display.glb for the official display window!
      const parts = [
        { file: 'bottom.glb', mat: this.matBottom, role: 'bottom' },
        { file: 'middle.glb', mat: this.matMiddle, role: 'middle' },
        { file: 'face_display.glb', mat: this.matFace, role: 'face' },
        { file: 'top.glb', mat: this.matTop, role: 'top' },
        { file: 'wheels.glb', mat: null, role: 'wheels' }
      ];

      const cadContainer = new T.Group();
      cadContainer.name = 'cadContainer';
      cadContainer.scale.set(DIM.CAD_SCALE, DIM.CAD_SCALE, DIM.CAD_SCALE); // 1mm -> 0.1cm

      let loadedCount = 0;
      parts.forEach(p => {
        loader.load(basePath + p.file, (gltf) => {
          const obj = gltf.scene;

          if (p.role === 'wheels') {
            let meshIdx = 0;
            let origTireMesh = null;
            const otherMeshes = [];

            obj.traverse(child => {
              if (child.isMesh) {
                // Ensure sharp planar CAD normals
                if (child.geometry.index) {
                  child.geometry = child.geometry.toNonIndexed();
                }
                child.geometry.computeVertexNormals();

                if (meshIdx === 0) {
                  origTireMesh = child;
                } else if (meshIdx === 1) {
                  child.material = this.matChrome;
                  otherMeshes.push(child);
                } else {
                  child.material = this.matMiddle;
                  otherMeshes.push(child);
                }
                meshIdx++;
              }
            });

            // Split tire mesh into left and right wheels
            if (origTireMesh) {
              const origGeo = origTireMesh.geometry;
              const pos = origGeo.attributes.position;
              const leftPositions = [];
              const rightPositions = [];

              for (let i = 0; i < pos.count; i += 3) {
                const x0 = pos.getX(i);
                const isLeft = x0 < 0;
                const target = isLeft ? leftPositions : rightPositions;
                for (let j = 0; j < 3; j++) {
                  target.push(pos.getX(i + j), pos.getY(i + j), pos.getZ(i + j));
                }
              }

              // Left Wheel (centered around its axle in cm)
              const leftGeo = new T.BufferGeometry();
              leftGeo.setAttribute('position', new T.Float32BufferAttribute(leftPositions, 3));
              leftGeo.computeVertexNormals();
              // In mm: translate by (+40.612, +10.0, +23.0)
              leftGeo.translate(40.612, 10.0, 23.0);
              const leftMesh = new T.Mesh(leftGeo, this.matTire);
              leftMesh.scale.set(DIM.CAD_SCALE, DIM.CAD_SCALE, DIM.CAD_SCALE);
              leftMesh.castShadow = true;
              leftMesh.receiveShadow = true;
              this.leftWheelPivot.add(leftMesh);

              // Right Wheel (centered around its axle in cm)
              const rightGeo = new T.BufferGeometry();
              rightGeo.setAttribute('position', new T.Float32BufferAttribute(rightPositions, 3));
              rightGeo.computeVertexNormals();
              // In mm: translate by (-40.612, +10.0, +23.0)
              rightGeo.translate(-40.612, 10.0, 23.0);
              const rightMesh = new T.Mesh(rightGeo, this.matTire);
              rightMesh.scale.set(DIM.CAD_SCALE, DIM.CAD_SCALE, DIM.CAD_SCALE);
              rightMesh.castShadow = true;
              rightMesh.receiveShadow = true;
              this.rightWheelPivot.add(rightMesh);
            }

            // Other caster & bracket meshes stay in CAD container
            otherMeshes.forEach(m => {
              m.castShadow = true;
              m.receiveShadow = true;
              cadContainer.add(m);
            });

          } else {
            // Body parts: convert to non-indexed for razor-sharp flat faces without pinching
            obj.traverse(child => {
              if (child.isMesh) {
                if (child.geometry.index) {
                  child.geometry = child.geometry.toNonIndexed();
                }
                child.geometry.computeVertexNormals();
                child.castShadow = true;
                child.receiveShadow = true;
                if (p.mat) child.material = p.mat;
                if (p.role === 'middle' || p.role === 'face') {
                  this.bodyParts.push(child);
                }
              }
            });
            cadContainer.add(obj);
          }

          loadedCount++;
          if (loadedCount === parts.length) {
            this._mountDisplayScreen(cadContainer);
            this.chassisGroup.add(cadContainer);

            // Remove fallback placeholder
            if (this.fallbackGroup) {
              this.chassisGroup.remove(this.fallbackGroup);
              this.fallbackGroup = null;
            }
            this.isCADLoaded = true;
          }
        }, undefined, (err) => {
          console.warn('Failed loading CAD part: ' + p.file, err);
        });
      });
    }

    /* -------------------------------------------------------------
     * 5. Mount Screen inside Face Window (X:[-20, 20], Y:[2.5, 28.5])
     * ----------------------------------------------------------- */
    _mountDisplayScreen(parent) {
      // In mm CAD coordinates: W = 39.5, H = 25.5, Y = 15.5, Z = 41.2
      const screenW = 39.5;
      const screenH = 25.5;
      const screenGeo = new T.PlaneGeometry(screenW, screenH);
      const screenMesh = new T.Mesh(screenGeo, this.matScreen);
      screenMesh.position.set(0, 15.5, 41.2);
      parent.add(screenMesh);

      // Glass cover
      const glassGeo = new T.PlaneGeometry(screenW, screenH);
      const glassMesh = new T.Mesh(glassGeo, this.matGlass);
      glassMesh.position.set(0, 15.5, 41.4);
      parent.add(glassMesh);
    }

    /* -------------------------------------------------------------
     * 6. Public Controls & Animation Loops
     * ----------------------------------------------------------- */

    /**
     * Set motor speeds for left and right wheels (-100 to +100)
     */
    setSpeed(speedL, speedR) {
      this.speedL = Math.max(-100, Math.min(100, speedL));
      this.speedR = Math.max(-100, Math.min(100, speedR));
    }

    /**
     * Switch robot facial emotion
     * @param {string} emotion - 'idle' | 'happy' | 'listening' | 'thinking' | 'speaking' | 'drive_fwd' | 'drive_rev' | 'turn_left' | 'turn_right' | 'dizzy' | 'obstacle' | 'sleepy'
     */
    setEmotion(emotion) {
      const newEmo = String(emotion).toLowerCase();
      if (this.currentEmotion !== newEmo) {
        this.currentEmotion = newEmo;
        this.screenNeedsRedraw = true;
      }
    }

    /**
     * Switch Chassis Color Palette
     * @param {string|number} colorOrPreset - 'white' | 'graphite' | 'mint' | 'yellow' or Hex
     */
    setChassisColor(colorOrPreset) {
      const presets = {
        white: 0x549ef7,     // Official HP Robotic Blue for middle shell
        graphite: 0x1e293b,
        mint: 0x10b981,
        yellow: 0xf59e0b
      };
      const col = presets[colorOrPreset] !== undefined ? presets[colorOrPreset] : colorOrPreset;
      this.matMiddle.color.set(col);
      this.matFace.color.set(col);
      this.bodyParts.forEach(m => {
        if (m.material) m.material.color.set(col);
      });
    }

    /**
     * Control Headlights & Screen Backlight
     */
    setLights({ headlights = true } = {}) {
      if (this.matScreen) {
        this.matScreen.emissiveIntensity = headlights ? 0.95 : 0.4;
      }
    }

    /**
     * Call every frame in requestAnimationFrame
     * @param {number} deltaSeconds
     */
    update(deltaSeconds = 0.016) {
      const dt = Math.min(deltaSeconds, 0.1);

      // 1. Wheel Rotation (Differential Drive)
      const radPerSecL = (this.speedL / 100) * 10;
      const radPerSecR = (this.speedR / 100) * 10;

      this.wheelAngleL += radPerSecL * dt;
      this.wheelAngleR += radPerSecR * dt;

      if (this.leftWheelPivot) this.leftWheelPivot.rotation.x = this.wheelAngleL;
      if (this.rightWheelPivot) this.rightWheelPivot.rotation.x = this.wheelAngleR;

      // 2. Chassis Pitch Inertia (Acceleration / Braking Tilt)
      const avgForwardSpeed = (this.speedL + this.speedR) / 2;
      const targetForwardTilt = -(avgForwardSpeed / 100) * 0.06;
      this.targetPitch = targetForwardTilt;
      this.currentPitch += (this.targetPitch - this.currentPitch) * (1 - Math.exp(-8 * dt));

      // 3. Chassis Roll Inertia (Cornering Centrifugal Lean)
      const turnDiff = (this.speedR - this.speedL) / 100;
      this.targetRoll = turnDiff * 0.07;
      this.currentRoll += (this.targetRoll - this.currentRoll) * (1 - Math.exp(-10 * dt));

      // 4. Idle Breathing Bobbing
      const isStationary = Math.abs(this.speedL) < 1 && Math.abs(this.speedR) < 1;
      let idleBobY = 0;
      let idleBobPitch = 0;

      if (isStationary) {
        this.idleTime += dt;
        idleBobY = Math.sin(this.idleTime * Math.PI) * 0.04;
        idleBobPitch = Math.cos(this.idleTime * Math.PI) * 0.01;
      } else {
        this.idleTime = 0;
      }

      this.chassisGroup.position.y = DIM.AXLE_Y + idleBobY;
      this.chassisGroup.rotation.x = this.currentPitch + idleBobPitch;
      this.chassisGroup.rotation.z = this.currentRoll;

      // 5. Intelligent Eye Blinking & Gaze Shift (Zero lag: only redraws when blinking/shifting)
      if (this.currentEmotion === 'idle' || this.currentEmotion === 'drive_fwd') {
        this.nextBlinkTime -= dt;
        if (this.nextBlinkTime <= 0) {
          this.eyeBlink = 1.0;
          this._redrawScreen();
          setTimeout(() => {
            this.eyeBlink = 0.0;
            // Gaze tracking: steer towards direction of travel
            const steerDiff = (this.speedR - this.speedL) / 100;
            this.pupilX = steerDiff * 30 + (Math.random() - 0.5) * 12;
            this.pupilY = (Math.random() - 0.5) * 8;
            this._redrawScreen();
            this.nextBlinkTime = 2.5 + Math.random() * 2.5;
          }, 120);
        }
      } else if (this.currentEmotion === 'dizzy') {
        // Spiral animation throttled to ~20 FPS for peak smoothness
        if (this.animFrame % 3 === 0) {
          this._redrawScreen();
        }
      }

      if (this.screenNeedsRedraw) {
        this._redrawScreen();
      }

      this.animFrame++;
    }

    /**
     * Cleanup and dispose VRAM resources
     */
    dispose() {
      if (this.screenTex) this.screenTex.dispose();
      [
        this.matTop, this.matMiddle, this.matFace, this.matBottom,
        this.matTire, this.matChrome, this.matScreen, this.matGlass
      ].forEach(mat => { if (mat) mat.dispose(); });
    }
  }

  return OttoRobot3D;
}));
