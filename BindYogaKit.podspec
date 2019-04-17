podspec = Pod::Spec.new do |spec|
  spec.name = 'BindYogaKit'
  spec.version = '1.9.0'
  spec.license =  { :type => 'MIT', :file => "LICENSE" }
  spec.homepage = 'https://facebook.github.io/yoga/'
  spec.documentation_url = 'https://facebook.github.io/yoga/docs/api/yogakit/'

  spec.summary = 'BindYoga is a cross-platform layout engine which implements Flexbox.'
  spec.description = 'BindYoga is a cross-platform layout engine enabling maximum collaboration within your team by implementing an API many designers are familiar with, and opening it up to developers across different platforms.'

  spec.authors = 'Facebook'
  spec.source = {
    :git => 'https://github.com/facebook/yoga.git',
    :tag => spec.version.to_s,
  }

  spec.platform = :ios
  spec.ios.deployment_target = '8.0'
  spec.ios.frameworks = 'UIKit'

  spec.dependency 'BindYoga', '~> 1.9'
  spec.source_files = 'BindYogaKit/Source/*.{h,m,swift}'
  spec.public_header_files = 'BindYogaKit/Source/{BNDYGLayout,UIView+BindYoga}.h'
  spec.private_header_files = 'BindYogaKit/Source/BNDYGLayout+Private.h'
end

# See https://github.com/facebook/yoga/pull/366
podspec.attributes_hash["readme"] = "BindYogaKit/README.md"
podspec
