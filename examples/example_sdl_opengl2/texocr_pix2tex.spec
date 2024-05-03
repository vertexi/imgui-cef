# -*- mode: python ; coding: utf-8 -*-
from PyInstaller.utils.hooks import copy_metadata, collect_data_files, collect_all

binaries = []
modules = ['onnxruntime']
for module in modules:
    tmp_ret = collect_all(module)
    binaries += tmp_ret[1]

datas = []
datas += copy_metadata('tqdm')
datas += copy_metadata('regex')
datas += copy_metadata('requests')
datas += copy_metadata('packaging')
datas += copy_metadata('filelock')
datas += copy_metadata('numpy')
datas += copy_metadata('huggingface-hub')
datas += copy_metadata('safetensors')
datas += copy_metadata('pyyaml')
datas += copy_metadata('tokenizers')
datas += copy_metadata('torch')
datas += collect_data_files('transformers', include_py_files=True, includes=['**/*.py'])
datas += collect_data_files('tokenizers', include_py_files=True, includes=['**/*.py'])
datas += collect_data_files('torch', include_py_files=True, includes=['**/*.py'])


block_cipher = None


a = Analysis(
    ['texocr_pix2tex.py'],
    pathex=["C:\\Users\\Apoptosis\\.conda\\envs\\texocr\\Lib\\site-packages", "C:\\Users\\Apoptosis\\.conda\\envs\\texocr\\Lib"],
    binaries=binaries,
    datas=datas,
    hiddenimports=[],
    hookspath=[],
    hooksconfig={},
    runtime_hooks=[],
    excludes=[],
    win_no_prefer_redirects=False,
    win_private_assemblies=False,
    cipher=block_cipher,
    noarchive=False,
)
pyz = PYZ(a.pure, a.zipped_data, cipher=block_cipher)

exe = EXE(
    pyz,
    a.scripts,
    [],
    name='texocr_pix2tex',
    debug=False,
    bootloader_ignore_signals=False,
    strip=False,
    upx=True,
    upx_exclude=[],
    runtime_tmpdir=None,
    console=True,
    disable_windowed_traceback=False,
    argv_emulation=False,
    target_arch=None,
    codesign_identity=None,
    entitlements_file=None,
)
coll = COLLECT(exe,
    a.binaries,
    a.zipfiles,
    a.datas,
    strip=False,
    upx=True,
    upx_exclude=[],
    name='texocr_pix2tex'
)
