from __future__ import print_function
import argparse
import torch
from PIL import Image
from torchvision.transforms import ToTensor

import numpy as np

# Training settings
parser = argparse.ArgumentParser(description='PyTorch Super Res Example')
parser.add_argument('--input_image', type=str, required=True, help='input image to use')
parser.add_argument('--model', type=str, required=True, help='model file to use')
parser.add_argument('--output_filename', type=str, help='where to save the output image')
parser.add_argument('--cuda', action='store_true', help='use cuda')
opt = parser.parse_args()

print(opt)

img = Image.open(opt.input_image).convert('YCbCr')
y, cb, cr = img.split()

model = torch.load(opt.model)
img_to_tensor = ToTensor()
input = img_to_tensor(y).view(1, -1, y.size[1], y.size[0]) # SR 알고리즘은 y채널에만 적용한다

if opt.cuda:
    model = model.cuda()
    input = input.cuda()

out = model(input) # 현재의 out은 gpu에 연결된 형태
out = out.cpu() # out을 다시 cpu에 연결

out_img_y = out[0].detach().numpy()
out_img_y *= 255.0
out_img_y = out_img_y.clip(0, 255)
out_img_y = Image.fromarray(np.uint8(out_img_y[0]), mode='L')

# 애초에 훈련할 때도 y만 이용하며,  bicubic interploation도 y에만 적용되고, 그에 따른 output 결과도 y채널만 쓰기 때문에
# 진짜 이미지를 SR하기 위해서는 cb, cr 채널도 bicubic interploation 적용해서 y랑 합쳐줘야 한다.

out_img_cb = cb.resize(out_img_y.size, Image.BICUBIC) # 논문 p.9 보면 y만 SR 적용, 
out_img_cr = cr.resize(out_img_y.size, Image.BICUBIC) # cb, cr은 그냥 bicubic interploation 적용
                                                      
out_img = Image.merge('YCbCr', [out_img_y, out_img_cb, out_img_cr]).convert('RGB')

out_img.save(opt.output_filename)
print('output image saved to ', opt.output_filename)